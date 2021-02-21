/********************************************************************************
 *  File Name:
 *    freertos_thread.cpp
 *
 *  Description:
 *    Chimera thread implementation with FreeRTOS
 *
 *  2020-2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* STL Includes */
#include <cstring>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/system>
#include <Chimera/thread>
#include <Chimera/source/drivers/threading/common/threading_internal.hpp>

#if defined( USING_FREERTOS ) || defined( USING_FREERTOS_THREADS )

/* FreeRTOS Includes */
#include <FreeRTOS/FreeRTOS.h>
#include <FreeRTOS/task.h>
#include <FreeRTOS/semphr.h>

namespace Chimera::Thread
{
  /*-------------------------------------------------------------------------------
  Structures
  -------------------------------------------------------------------------------*/
  struct DelegateArgs
  {
    TaskDelegate pDelegate;
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


  bool sendTaskMsg( const TaskId id, const TaskMsg msg, const size_t timeout )
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
  Task::Task()
  {
    mTaskConfig = {};
    mTaskId     = Chimera::Thread::THREAD_ID_INVALID;
    mRunning    = false;
  }

  Task::Task( Task &&other )
  {
    mRunning      = other.mRunning;
    mTaskId       = other.mTaskId;
    mTaskConfig   = other.mTaskConfig;
    mNativeThread = std::move( other.mNativeThread );
  }


  Task::~Task()
  {
  }


  /*-------------------------------------------------
  Public Methods
  -------------------------------------------------*/
  void Task::initialize( TaskFuncPtr func, TaskArg arg, const Priority priority, const size_t stackWords,
                         const std::string_view name )
  {
    /*------------------------------------------------
    Copy the parameters
    ------------------------------------------------*/
    mTaskConfig.type                      = TaskInitType::DYNAMIC;
    mTaskConfig.function.type             = FunctorType::C_STYLE;
    mTaskConfig.function.callable.pointer = func;
    mTaskConfig.arg                       = arg;
    mTaskConfig.priority                  = priority;
    mTaskConfig.stackWords                = stackWords;
    mTaskConfig.name                      = name.cbegin();
  }


  void Task::initialize( TaskDelegate func, TaskArg arg, const Priority priority, const size_t stackWords,
                         const std::string_view name )
  {
    /*------------------------------------------------
    Copy the parameters
    ------------------------------------------------*/
    mTaskConfig.type                       = TaskInitType::DYNAMIC;
    mTaskConfig.function.type              = FunctorType::DELEGATE;
    mTaskConfig.function.callable.delegate = func;
    mTaskConfig.arg                        = arg;
    mTaskConfig.priority                   = priority;
    mTaskConfig.stackWords                 = stackWords;
    mTaskConfig.name                       = name.cbegin();
  }


  TaskId Task::start()
  {
    /*------------------------------------------------
    Actually create the thread. If the scheduler is
    running, it will immediately start. Otherwise it
    will wait until scheduler executes.
    ------------------------------------------------*/
    BaseType_t result = 0;

    switch ( mTaskConfig.type )
    {
      case TaskInitType::DYNAMIC:
        result = this->startDynamic();
        break;

      case TaskInitType::STATIC:
        result = this->startStatic();
        break;

      case TaskInitType::RESTRICTED:
        result = this->startRestricted();
        break;

      default:
        RT_HARD_ASSERT( false );
        break;
    }

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


  void Task::suspend()
  {
    lookup_handle();
    vTaskSuspend( mNativeThread );
  }


  void Task::resume()
  {
    lookup_handle();
    vTaskResume( mNativeThread );
  }


  void Task::join()
  {
    /*-------------------------------------------------
    Instruct the thread to exit if it's already running
    -------------------------------------------------*/
    if ( joinable() )
    {
      lookup_handle();
      sendTaskMsg( mTaskId, ITCMsg::TSK_MSG_EXIT, TIMEOUT_DONT_WAIT );
      vTaskDelete( mNativeThread );
    }

    /*-------------------------------------------------
    Clean up Chimera's notion of the thread's existence
    -------------------------------------------------*/
    unregisterThread( mTaskId );
  }


  bool Task::joinable()
  {
    return mRunning && ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING );
  }


  detail::native_thread_handle_type Task::native_handle()
  {
    lookup_handle();
    return mNativeThread;
  }

  detail::native_thread_id Task::native_id()
  {
    return mTaskId;
  }


  /*-------------------------------------------------------------------------------
  Task: Private Methods
  -------------------------------------------------------------------------------*/
  void Task::lookup_handle()
  {
    /*-------------------------------------------------
    According to documentation, this operation can take
    a while, so only do it once.
    -------------------------------------------------*/
    if ( !mNativeThread )
    {
      mNativeThread = xTaskGetHandle( mTaskConfig.name.cbegin() );
    }
  }


  int Task::startStatic()
  {
    /*-------------------------------------------------
    Entrance checks
    -------------------------------------------------*/
    static_assert( sizeof( StaticTask_t ) % sizeof( portSTACK_TYPE ) == 0 );
    RT_HARD_ASSERT( mTaskConfig.specialization.staticTask.stackBuffer );
    RT_HARD_ASSERT( mTaskConfig.specialization.staticTask.stackSize > sizeof( StaticTask_t ) );

    /*-------------------------------------------------
    Create a StaticTask_t control block on the heap
    -------------------------------------------------*/
    size_t allocByteSize  = sizeof( StaticTask_t );
    void *rawBuffer       = mTaskConfig.specialization.staticTask.stackBuffer;
    StaticTask_t *taskCB  = new ( rawBuffer ) StaticTask_t();
    StackType_t *taskBuff = reinterpret_cast<StackType_t*>( reinterpret_cast<uint8_t *>( rawBuffer ) + allocByteSize );

    /*-------------------------------------------------
    Adjust the sizing. Stack depth at this point is the
    number of words in the stack, not number of bytes.
    -------------------------------------------------*/
    mTaskConfig.stackWords -= ( allocByteSize / sizeof( portSTACK_TYPE ) );
    mTaskConfig.specialization.staticTask.stackSize -= allocByteSize;

    /*-------------------------------------------------
    Create the tasks
    -------------------------------------------------*/
    if ( mTaskConfig.function.type == FunctorType::C_STYLE )
    {
      mNativeThread = xTaskCreateStatic( mTaskConfig.function.callable.pointer, mTaskConfig.name.data(),
                                         static_cast<configSTACK_DEPTH_TYPE>( mTaskConfig.stackWords ), mTaskConfig.arg,
                                         static_cast<UBaseType_t>( mTaskConfig.priority ), taskBuff, taskCB );
    }
    else  // FunctorType::DELEGATE
    {
      /*-------------------------------------------------
      Encapsulate the delegate into a helper structure to
      pass multiple arguments to the lambda below.
      -------------------------------------------------*/
      DelegateArgs helperArgs;
      helperArgs.pArguments = mTaskConfig.arg;
      helperArgs.pDelegate  = mTaskConfig.function.callable.delegate;

      /*-------------------------------------------------
      Use a lambda as an impromptu C-Style wrapper for
      calling the delegate function.
      -------------------------------------------------*/
      mNativeThread = xTaskCreateStatic(
          []( void *o ) {
            TaskConfig *proxy = reinterpret_cast<TaskConfig *>( o );
            proxy->function.callable.delegate( proxy->arg );
          },
          mTaskConfig.name.data(), static_cast<configSTACK_DEPTH_TYPE>( mTaskConfig.stackWords ), &helperArgs,
          static_cast<UBaseType_t>( mTaskConfig.priority ), taskBuff, taskCB );
    }

    return ( mNativeThread ? pdPASS : errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY );
  }


  int Task::startDynamic()
  {
    BaseType_t result = 0;

    if ( mTaskConfig.function.type == FunctorType::C_STYLE )
    {
      result = xTaskCreate( mTaskConfig.function.callable.pointer, mTaskConfig.name.data(),
                            static_cast<configSTACK_DEPTH_TYPE>( mTaskConfig.stackWords ), mTaskConfig.arg,
                            static_cast<UBaseType_t>( mTaskConfig.priority ), &mNativeThread );
    }
    else  // FunctorType::DELEGATE
    {
      /*-------------------------------------------------
      Encapsulate the delegate into a helper structure to
      pass multiple arguments to the lambda below.
      -------------------------------------------------*/
      DelegateArgs helperArgs;
      helperArgs.pArguments = mTaskConfig.arg;
      helperArgs.pDelegate  = mTaskConfig.function.callable.delegate;

      /*-------------------------------------------------
      Use a lambda as an impromptu C-Style wrapper for
      calling the delegate function.
      -------------------------------------------------*/
      result = xTaskCreate(
          []( void *o ) {
            TaskConfig *proxy = reinterpret_cast<TaskConfig *>( o );
            proxy->function.callable.delegate( proxy->arg );
          },
          mTaskConfig.name.data(), static_cast<configSTACK_DEPTH_TYPE>( mTaskConfig.stackWords ), &mTaskConfig,
          static_cast<UBaseType_t>( mTaskConfig.priority ), &mNativeThread );
    }

    return result;
  }


  int Task::startRestricted()
  {
    // Not supported yet
    RT_HARD_ASSERT( false );
    return errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY;
  }

  /*-------------------------------------------------------------------------------
  this_thread Namespace
  -------------------------------------------------------------------------------*/
  void this_thread::yield()
  {
    taskYIELD();
  }


  void this_thread::suspend()
  {
    vTaskSuspend( xTaskGetCurrentTaskHandle() );
  }


  TaskId this_thread::id()
  {
    return getIdFromNativeHandle( xTaskGetCurrentTaskHandle() );
  }


  bool this_thread::receiveTaskMsg( TaskMsg &msg, const size_t timeout )
  {
    msg = static_cast<TaskMsg>( ulTaskNotifyTake( pdTRUE, pdMS_TO_TICKS( timeout ) ) );
    return !( msg == ITCMsg::TSK_MSG_NOP );
  }

}  // namespace Chimera::Thread

#endif /* FREERTOS */
