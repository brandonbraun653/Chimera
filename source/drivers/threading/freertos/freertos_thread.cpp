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

#if defined( USING_FREERTOS ) || defined( USING_FREERTOS_THREADS )

/* FreeRTOS Includes */
#include <FreeRTOS/FreeRTOS.h>
#include <FreeRTOS/task.h>
#include <FreeRTOS/semphr.h>

namespace Chimera::Threading
{
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
  }


  size_t schedulerResolution()
  {
    return ( configTICK_RATE_HZ / 1000 );
  }


  int hardwareConcurrency()
  {
    return Chimera::System::maxConcurrentThreads();
  }


  /*-------------------------------------------------------------------------------
  Thread Class Implementation
  -------------------------------------------------------------------------------*/
  Thread::Thread() : mFunc( nullptr ), mFuncArg( nullptr ), mThread( nullptr )
  {
    mThreadName.fill( 0 );
  }


  Thread::Thread( const Thread &other ) :
      mThread( other.mThread ), mFunc( other.mFunc ), mFuncArg( other.mFuncArg ), mPriority( other.mPriority ),
      mStackDepth( other.mStackDepth )
  {
    copy_thread_name( other.name() );
  }


  Thread::Thread( Thread &&other ) :
      mThread( other.mThread ), mFunc( other.mFunc ), mFuncArg( other.mFuncArg ), mPriority( other.mPriority ),
      mStackDepth( other.mStackDepth )
  {
    copy_thread_name( other.name() );
  }


  Thread::~Thread()
  {
  }


  void Thread::initialize( ThreadFunctPtr func, ThreadArg arg, const Priority priority, const size_t stackDepth,
                           const std::string_view name )
  {
    /*------------------------------------------------
    Copy the parameters
    ------------------------------------------------*/
    mFunc       = func;
    mFuncArg    = arg;
    mPriority   = priority;
    mStackDepth = stackDepth;
    mThread     = nullptr;
    copy_thread_name( name );
  }


  void Thread::start()
  {
    /*------------------------------------------------
    Actually create the thread. If the scheduler is running already, it will
    immediately start. Otherwise it will wait until scheduler executes.
    ------------------------------------------------*/
    auto result = xTaskCreate( mFunc, mThreadName.data(), static_cast<configSTACK_DEPTH_TYPE>( mStackDepth ), mFuncArg,
                               static_cast<UBaseType_t>( mPriority ), &mThread );

    /*-------------------------------------------------
    No memory to create the thread is really bad. Make
    it obvious that a critical failure occurred.
    -------------------------------------------------*/
    if ( result == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY )
    {
      Chimera::insert_debug_breakpoint();
      Chimera::System::softwareReset();
    }

    /*-------------------------------------------------
    Grab the handler assigned to the thread
    -------------------------------------------------*/
    if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
    {
      lookup_handle();
    }
  }


  void Thread::suspend()
  {
    lookup_handle();
    vTaskSuspend( mThread );
  }


  void Thread::resume()
  {
    lookup_handle();
    vTaskResume( mThread );
  }


  void Thread::join()
  {
    lookup_handle();
    vTaskDelete( mThread );
  }


  bool Thread::joinable()
  {
    return true;
  }


  detail::native_thread_handle_type Thread::native_handle()
  {
    lookup_handle();
    return mThread;
  }


  void Thread::lookup_handle()
  {
    /*-------------------------------------------------
    According to documentation, this operation can take
    a while, so only do it once.
    -------------------------------------------------*/
    if ( !mThread )
    {
      mThread = xTaskGetHandle( mThreadName.cbegin() );
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
}  // namespace Chimera::Threading

#endif /* FREERTOS */
