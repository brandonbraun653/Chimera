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
  Thread::Thread() : mFunc( nullptr ), mFuncArg( nullptr )
  {
    mThreadName.fill( 0 );
  }


  Thread::Thread( Thread &&other ) : mFunc( other.mFunc ), mFuncArg( other.mFuncArg )
  {
    mThreadName.fill( 0 );
  }


  Thread::~Thread()
  {
  }


  void Thread::initialize( ThreadFunctPtr func, ThreadArg arg, const Priority priority, const size_t stackDepth,
                           const std::string_view name )
  {
    /*------------------------------------------------
    Copy out the string data into the name
    ------------------------------------------------*/
    size_t copyLen = name.length();
    if ( copyLen > MAX_NAME_LEN )
    {
      copyLen = MAX_NAME_LEN;
    }

    mThreadName.fill( 0 );
    memcpy( mThreadName.data(), name.data(), copyLen );

    /*------------------------------------------------
    Copy the additional parameters
    ------------------------------------------------*/
    mFunc       = func;
    mFuncArg    = arg;
    mPriority   = priority;
    mStackDepth = stackDepth;
  }


  void Thread::start()
  {
    /*------------------------------------------------
    Actually create the thread. If the scheduler is running already, it will
    immediately start. Otherwise it will wait until scheduler executes.
    ------------------------------------------------*/
    xTaskCreate( mFunc, mThreadName.data(), static_cast<configSTACK_DEPTH_TYPE>( mStackDepth ), mFuncArg,
                 static_cast<UBaseType_t>( mPriority ), &mThread );
  }


  void Thread::join()
  {
    vTaskDelete( xTaskGetCurrentTaskHandle() );
  }


  bool Thread::joinable()
  {
    return true;
  }


  detail::native_thread_handle_type Thread::native_handle()
  {
    return xTaskGetCurrentTaskHandle();
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
}  // namespace Chimera::Threading

#endif /* FREERTOS */
