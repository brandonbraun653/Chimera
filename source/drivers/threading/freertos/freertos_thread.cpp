/********************************************************************************
 *  File Name:
 *    freertos_thread.cpp
 *
 *  Description:
 *    Chimera thread implementation with FreeRTOS
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* STL Includes */
#include <cstring>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/system>
#include <Chimera/thread>
#include <Chimera/source/drivers/threading/threading_thread_internal.hpp>

#if defined( USING_FREERTOS ) || defined( USING_FREERTOS_THREADS )

/* FreeRTOS Includes */
#include <FreeRTOS/FreeRTOS.h>
#include <FreeRTOS/task.h>
#include <FreeRTOS/semphr.h>

namespace Chimera::Threading
{
  /*-------------------------------------------------------------------------------
  Structures
  -------------------------------------------------------------------------------*/
  struct DelegateArgs
  {
    ThreadDelegate pDelegate;
    void *pArguments;
  };

  /*-------------------------------------------------------------------------------
  User Interface
  -------------------------------------------------------------------------------*/
  void startScheduler()
  {
    vTaskStartScheduler();
  }


  void stopScheduler()
  {
    // FreeRTOS doesn't exactly stop once it gets going
    Chimera::insert_debug_breakpoint();
  }


  size_t schedulerResolution()
  {
    return ( configTICK_RATE_HZ / 1000 );
  }


  int hardwareConcurrency()
  {
    return Chimera::System::maxConcurrentThreads();
  }


  bool sendTaskMsg( const ThreadId id, const ThreadMsg msg, const size_t timeout )
  {
    /*-------------------------------------------------
    Does the task exist?
    -------------------------------------------------*/
    if ( auto thread = getThread( id ); thread != nullptr )
    {
      BaseType_t result;
      if ( Chimera::System::inISR() )
      {
        result = xTaskNotifyFromISR( thread->native_handle(), msg, eSetValueWithOverwrite, nullptr );
      }
      else
      {
        result = xTaskNotify( thread->native_handle(), msg, eSetValueWithOverwrite );
      }

      return ( result == pdPASS );
    }
    else
    {
      return false;
    }
  }

  /*-------------------------------------------------------------------------------
  Thread Class Implementation
  -------------------------------------------------------------------------------*/
  /*-------------------------------------------------
  Ctors/Dtors
  -------------------------------------------------*/
  Thread::Thread() : mFunc( {} ), mNativeThread( nullptr ), mThreadId( THREAD_ID_INVALID ), mRunning( false )
  {
    mName.fill( 0 );
  }

  Thread::Thread( Thread &&other ) :
      mNativeThread( other.mNativeThread ), mFunc( other.mFunc ), mPriority( other.mPriority ),
      mStackDepth( other.mStackDepth ), mThreadId( other.mThreadId ), mRunning( mRunning )
  {
    copy_thread_name( other.name() );
  }


  Thread::~Thread()
  {
  }


  /*-------------------------------------------------
  Public Methods
  -------------------------------------------------*/
  void Thread::initialize( ThreadFunctPtr func, ThreadArg arg, const Priority priority, const size_t stackDepth,
                           const std::string_view name )
  {
    /*------------------------------------------------
    Copy the parameters
    ------------------------------------------------*/
    mFunc.type             = FunctorType::C_STYLE;
    mFunc.function.pointer = func;
    mFunc.arg              = arg;
    mPriority              = priority;
    mStackDepth            = stackDepth;
    mNativeThread          = nullptr;
    copy_thread_name( name );
  }


  void Thread::initialize( ThreadDelegate func, ThreadArg arg, const Priority priority, const size_t stackDepth,
                           const std::string_view name )
  {
    /*------------------------------------------------
    Copy the parameters
    ------------------------------------------------*/
    mFunc.type              = FunctorType::DELEGATE;
    mFunc.function.delegate = func;
    mFunc.arg               = arg;
    mPriority               = priority;
    mStackDepth             = stackDepth;
    mNativeThread           = nullptr;
    copy_thread_name( name );
  }


  ThreadId Thread::start()
  {
    /*------------------------------------------------
    Actually create the thread. If the scheduler is
    running, it will immediately start. Otherwise it
    will wait until scheduler executes.
    ------------------------------------------------*/
    BaseType_t result;
    if( mFunc.type == FunctorType::C_STYLE )
    {
      result = xTaskCreate( mFunc.function.pointer, mName.data(), static_cast<configSTACK_DEPTH_TYPE>( mStackDepth ), mFunc.arg,
                            static_cast<UBaseType_t>( mPriority ), &mNativeThread );
    }
    else // FunctorType::DELEGATE
    {
      /*-------------------------------------------------
      Encapsulate the delegate into a helper structure to
      pass multiple arguments to the lambda below.
      -------------------------------------------------*/
      DelegateArgs helperArgs;
      helperArgs.pArguments = mFunc.arg;
      helperArgs.pDelegate = mFunc.function.delegate;

      /*-------------------------------------------------
      Use a lambda as an impromptu C-Style wrapper for
      calling the delegate function.
      -------------------------------------------------*/
      result = xTaskCreate(
          []( void *o ) {
            UserFunction *proxy = reinterpret_cast<UserFunction *>( o );
            proxy->function.delegate( proxy->arg );
          },
          mName.data(), static_cast<configSTACK_DEPTH_TYPE>( mStackDepth ), &mFunc, static_cast<UBaseType_t>( mPriority ),
          &mNativeThread );
    }
    // Ensure this function is handling all cases...
    static_assert( static_cast<size_t>( FunctorType::NUM_OPTIONS ) == 2 );

    /*-------------------------------------------------
    No memory to create the thread is really bad. Make
    it obvious that a critical failure occurred.
    -------------------------------------------------*/
    if ( result == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY )
    {
      Chimera::insert_debug_breakpoint();
      Chimera::System::softwareReset();
    }
    else
    {
      mRunning = true;
    }

    /*-------------------------------------------------
    Grab the handler assigned to the thread
    -------------------------------------------------*/
    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
    {
      lookup_handle();
    }

    /*-------------------------------------------------
    Register the thread with the system
    -------------------------------------------------*/
    return registerThread( std::move( *this ) );
  }


  void Thread::suspend()
  {
    lookup_handle();
    vTaskSuspend( mNativeThread );
  }


  void Thread::resume()
  {
    lookup_handle();
    vTaskResume( mNativeThread );
  }


  void Thread::join()
  {
    /*-------------------------------------------------
    Instruct the thread to exit if it's already running
    -------------------------------------------------*/
    if ( joinable() )
    {
      lookup_handle();
      sendTaskMsg( mThreadId, ITCMsg::TSK_MSG_EXIT, TIMEOUT_DONT_WAIT );
      vTaskDelete( mNativeThread );
    }

    /*-------------------------------------------------
    Clean up Chimera's notion of the thread's existence
    -------------------------------------------------*/
    unregisterThread( mThreadId );
  }


  bool Thread::joinable()
  {
    return mRunning && ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING );
  }


  detail::native_thread_handle_type Thread::native_handle()
  {
    lookup_handle();
    return mNativeThread;
  }


  /*-------------------------------------------------
  Private Methods
  -------------------------------------------------*/
  void Thread::lookup_handle()
  {
    /*-------------------------------------------------
    According to documentation, this operation can take
    a while, so only do it once.
    -------------------------------------------------*/
    if ( !mNativeThread )
    {
      mNativeThread = xTaskGetHandle( mName.cbegin() );
    }
  }

  /*-------------------------------------------------------------------------------
  this_thread Namespace
  -------------------------------------------------------------------------------*/
  void this_thread::sleep_for( const size_t timeout )
  {
    vTaskDelay( pdMS_TO_TICKS( timeout ) );
  }


  void this_thread::sleep_until( const size_t timeout )
  {
    auto now = Chimera::millis();
    if ( timeout > now )
    {
      vTaskDelay( pdMS_TO_TICKS( timeout - now ) );
    }
  }


  void this_thread::yield()
  {
    taskYIELD();
  }


  void this_thread::suspend()
  {
    vTaskSuspend( xTaskGetCurrentTaskHandle() );
  }


  ThreadId this_thread::id()
  {
    return getIdFromNativeHandle( xTaskGetCurrentTaskHandle() );
  }


  bool this_thread::receiveTaskMsg( ThreadMsg &msg, const size_t timeout )
  {
    msg = static_cast<ThreadMsg>( ulTaskNotifyTake( pdTRUE, pdMS_TO_TICKS( timeout ) ) );
    return !( msg == ITCMsg::TSK_MSG_NOP );
  }

}  // namespace Chimera::Threading

#endif /* FREERTOS */
