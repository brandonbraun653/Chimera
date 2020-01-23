/********************************************************************************
 *   File Name:
 *
 *
 *   Description:
 *
 *
 *   2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */
#include <cstring>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/system.hpp>
#include <Chimera/threading/extensions.hpp>
#include <Chimera/threading/mutex.hpp>
#include <Chimera/threading/thread.hpp>

/* FreeRTOS Includes */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#if defined( USING_FREERTOS ) || defined( USING_FREERTOS_THREADS )

namespace Chimera::Threading
{
  /************************************************************************/
  /*                             EXTENSIONS                               */
  /************************************************************************/
  Lockable::Lockable()
  {
  }

  Lockable::~Lockable()
  {
  }

  void Lockable::lock()
  {
    mutex.lock();
  }

  void Lockable::lockFromISR()
  {
    mutex.lock();
  }

  bool Lockable::try_lock_for( const size_t timeout )
  {
    return mutex.try_lock_for( timeout );
  }

  void Lockable::unlock()
  {
    mutex.unlock();
  }

  void Lockable::unlockFromISR()
  {
    mutex.unlock();
  }

  /************************************************************************/
  /*                                MUTEX                                 */
  /************************************************************************/

  /*------------------------------------------------
  Mutex Implementation
  ------------------------------------------------*/
  Mutex::Mutex()
  {
    _mtx = xSemaphoreCreateMutex();
  }

  Mutex::~Mutex()
  {
    vSemaphoreDelete( _mtx );
  }

  void Mutex::lock()
  {
    xSemaphoreTake( _mtx, portMAX_DELAY );
  }

  bool Mutex::try_lock()
  {
    return ( xSemaphoreTake( _mtx, 0 ) == pdPASS );
  }

  void Mutex::unlock()
  {
    xSemaphoreGive( _mtx );
  }

  /*------------------------------------------------
  Recursive Mutex Implementation
  ------------------------------------------------*/
  RecursiveMutex::RecursiveMutex()
  {
    _mtx = xSemaphoreCreateRecursiveMutex();
  }

  RecursiveMutex::~RecursiveMutex()
  {
    vSemaphoreDelete( _mtx );
  }

  void RecursiveMutex::lock()
  {
    xSemaphoreTakeRecursive( _mtx, portMAX_DELAY );
  }

  bool RecursiveMutex::try_lock()
  {
    return ( xSemaphoreTakeRecursive( _mtx, 0 ) == pdPASS );
  }

  void RecursiveMutex::unlock()
  {
    xSemaphoreGiveRecursive( _mtx );
  }

  /*------------------------------------------------
  Timed Mutex Implementation
  ------------------------------------------------*/
  TimedMutex::TimedMutex()
  {
    _mtx = xSemaphoreCreateMutex();
  }

  TimedMutex::~TimedMutex()
  {
    vSemaphoreDelete( _mtx );
  }

  void TimedMutex::lock()
  {
    xSemaphoreTake( _mtx, portMAX_DELAY );
  }

  bool TimedMutex::try_lock()
  {
    return ( xSemaphoreTake( _mtx, 0 ) == pdPASS );
  }

  bool TimedMutex::try_lock_for( const size_t timeout )
  {
    return ( xSemaphoreTake( _mtx, pdMS_TO_TICKS( timeout ) ) == pdPASS );
  }

  bool TimedMutex::try_lock_until( const size_t timeout )
  {
    auto now = Chimera::millis();
    return ( xSemaphoreTake( _mtx, pdMS_TO_TICKS( timeout - now ) ) == pdPASS );
  }

  void TimedMutex::unlock()
  {
    xSemaphoreGive( _mtx );
  }

  /*------------------------------------------------
  Recursive Timed Mutex Implementation
  ------------------------------------------------*/
  RecursiveTimedMutex::RecursiveTimedMutex()
  {
    _mtx = xSemaphoreCreateRecursiveMutex();
  }

  RecursiveTimedMutex::~RecursiveTimedMutex()
  {
    vSemaphoreDelete( _mtx );
  }

  void RecursiveTimedMutex::lock()
  {
    xSemaphoreTakeRecursive( _mtx, portMAX_DELAY );
  }

  bool RecursiveTimedMutex::try_lock()
  {
    return ( xSemaphoreTakeRecursive( _mtx, 0 ) == pdPASS );
  }

  bool RecursiveTimedMutex::try_lock_for( const size_t timeout )
  {
    return ( xSemaphoreTakeRecursive( _mtx, pdMS_TO_TICKS( timeout ) ) == pdPASS );
  }

  bool RecursiveTimedMutex::try_lock_until( const size_t timeout )
  {
    auto now = Chimera::millis();
    return ( xSemaphoreTakeRecursive( _mtx, pdMS_TO_TICKS( timeout - now ) ) == pdPASS );
  }

  void RecursiveTimedMutex::unlock()
  {
    xSemaphoreGiveRecursive( _mtx );
  }

  /************************************************************************/
  /*                               THREAD                                 */
  /************************************************************************/

  void startScheduler()
  {
    vTaskStartScheduler();
  }

  void stopScheduler()
  {

  }

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
    mFunc = func;
    mFuncArg = arg;
    mPriority = priority;
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

  Id Thread::get_id()
  {
    return Id();
  }

  detail::native_thread_handle_type Thread::native_handle()
  {
    return xTaskGetCurrentTaskHandle();
  }

  int Thread::hardware_concurrency()
  {
    return Chimera::System::maxConcurrentThreads();
  }

  Id this_thread::get_id()
  {
    return Id();
  }

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
}

#endif 