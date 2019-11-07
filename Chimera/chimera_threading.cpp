/********************************************************************************
 * File Name:
 *   threading.cpp
 *
 * Description:
 *   Implements the threading functions for Chimera
 *
 * 2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */
#include <cstring>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/threading.hpp>
#include <Chimera/watchdog.hpp>


/*------------------------------------------------
These bytes were generated from atmospheric noise, so they
are pretty random I guess. https://www.random.org/bytes/
------------------------------------------------*/
static constexpr uint32_t sMsgSetupComplete = 0xbf931c86;
static constexpr uint32_t sMsgSetupResume   = 0x2e526078;

static std::vector<Chimera::Threading::Thread_t *> sSystemThreadRegistry;
static TaskHandle_t sInitThreadHandle;
static bool sSetupCallbacksEnabled = false;

namespace Chimera::Threading 
{
    /*-------------------------------------------------
    Lockable Class Implementation
    -------------------------------------------------*/
    Lockable::Lockable()
    {
      recursive_mutex = xSemaphoreCreateRecursiveMutex();
    }

    Chimera::Status_t Lockable::lock( const size_t timeout_mS )
    {
      Chimera::Status_t error = Chimera::CommonStatusCodes::OK;

      if ( xSemaphoreTakeRecursive( recursive_mutex, pdMS_TO_TICKS( timeout_mS ) ) != pdPASS )
      {
        error = Chimera::CommonStatusCodes::FAIL;
      }

      return error;
    }

    Chimera::Status_t Lockable::lockFromISR( const size_t timeout_mS )
    {
      Chimera::Status_t error = Chimera::CommonStatusCodes::OK;

      if ( xSemaphoreTakeFromISR( recursive_mutex, nullptr ) != pdPASS )
      {
        error = Chimera::CommonStatusCodes::FAIL;
      }

      return error;
    }

    Chimera::Status_t Lockable::unlock()
    {
      Chimera::Status_t error = Chimera::CommonStatusCodes::OK;

      if ( xSemaphoreGiveRecursive( recursive_mutex ) != pdPASS )
      {
        error = Chimera::CommonStatusCodes::FAIL;
      }

      return error;
    }

    Chimera::Status_t Lockable::unlockFromISR()
    {
      Chimera::Status_t error = Chimera::CommonStatusCodes::OK;

      if ( xSemaphoreGiveFromISR( recursive_mutex, nullptr ) != pdPASS )
      {
        error = Chimera::CommonStatusCodes::FAIL;
      }

      return error; 
    }

    /*-------------------------------------------------
    Timed Lock Guard Implementation
    -------------------------------------------------*/
    LockGuard::LockGuard( Lockable &obj ) : lockable( obj )
    {
    }

    LockGuard::~LockGuard()
    {
      lockable.unlock();
    }

    bool LockGuard::lock()
    {
      return lock( TIMEOUT_DONT_WAIT );
    }

    bool LockGuard::lock( const size_t timeout )
    {
      return ( lockable.lock( timeout ) == Chimera::CommonStatusCodes::OK );
    }

    /**
     *  Implements a simple timeout while waiting for a newly created thread to
     *  complete its initialization sequence.
     *
     *  @return BaseType_t
     */
    static BaseType_t threadInitTimeout()
    {
      BaseType_t error = pdPASS;

      if ( sSetupCallbacksEnabled )
      {
        static constexpr uint32_t checkDelay_mS  = 10;
        static constexpr uint32_t initTimeout_mS = 10000;

        TickType_t lastTimeWoken = xTaskGetTickCount();
        uint32_t timeoutCounter  = 0;

        /*------------------------------------------------
        Wait for the thread currently being initialized to signal back that it's ready
        ------------------------------------------------*/
        while ( !ulTaskNotifyTake( pdTRUE, 0 ) || ( error == pdFAIL ) )
        {
          vTaskDelayUntil( &lastTimeWoken, pdMS_TO_TICKS( checkDelay_mS ) );
          timeoutCounter += checkDelay_mS;

          if ( timeoutCounter > initTimeout_mS )
          {
            error = pdFAIL;
          }
        }
      }


      return error;
    }

    /**
     *  Thread that performs the startup sequence on all threads registered
     *  before startScheduler() was called.
     *
     *  @return void
     */
    static void initThread( void *arguments )
    {
      BaseType_t error = pdPASS;
      Thread_t *thread;

      /*------------------------------------------------
      Initialize all threads internally registered in the system
      ------------------------------------------------*/
      for ( size_t i = 0; i < sSystemThreadRegistry.size(); i++ )
      {
        thread = sSystemThreadRegistry[ i ];
        error  = xTaskCreate( thread->func, thread->name, thread->stackDepth, thread->funcParams, thread->priority,
                              thread->handle );

        /*------------------------------------------------
        If you get stuck here, the current thread tried to allocate
        more memory than what is left in the FreeRTOS heap.
        ------------------------------------------------*/
        if ( error == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY )
        {
          // volatile size_t bytesRemaining __attribute( ( unused ) ) = xPortGetFreeHeapSize();
          Chimera::Watchdog::invokeTimeout();
        }

        /*------------------------------------------------
        Wait for the setup to complete. If you get stuck here, it's probably
        because you didn't use the signalSetupComplete() callback...
        ------------------------------------------------*/
        if ( threadInitTimeout() != pdPASS )
        {
          vTaskSuspendAll();
          Chimera::Watchdog::invokeTimeout();
        }
      }

      /*------------------------------------------------
      Resume all registered threads
      ------------------------------------------------*/
      if ( sSetupCallbacksEnabled )
      {
        for ( size_t i = 0; i < sSystemThreadRegistry.size(); i++ )
        {
          if ( sSystemThreadRegistry[ i ]->handle )
          {
            xTaskNotify( *sSystemThreadRegistry[ i ]->handle, sMsgSetupResume, eSetValueWithOverwrite );
          }
        }
      }

      /*------------------------------------------------
      Cleanly exit this initialization thread. It's not 
      needed any more.
      ------------------------------------------------*/
      sInitThreadHandle           = nullptr;
      sSetupCallbacksEnabled = false;
      vTaskDelete( NULL );
    }

    void startScheduler( const bool useSetupCallbacks )
    {
      sSetupCallbacksEnabled = useSetupCallbacks;
      xTaskCreate( initThread, "threading_init", 500, NULL, 1, &sInitThreadHandle );
      vTaskStartScheduler();
    }

    BaseType_t addThread( Thread_t *const threadArray, const uint8_t numThreads )
    {
      for ( uint8_t x = 0; x < numThreads; x++ )
      {
        sSystemThreadRegistry.push_back( &threadArray[ x ] );
      }

      return pdPASS;
    }

    BaseType_t addThread( TaskFunction_t threadFunc, const char *threadName, const uint16_t stackDepth,
                          void *const threadFuncParams, UBaseType_t threadPriority, TaskHandle_t *const threadHandle )
    {
      /*------------------------------------------------
      Default to pass so the thread registration will take 
      place later if the scheduler isn't running yet.
      ------------------------------------------------*/
      BaseType_t error = pdPASS;

      /*------------------------------------------------
      Allocate enough space for the internally managed thread handle 
      ------------------------------------------------*/
      Thread_t *newThread   = new Thread_t;
      newThread->func       = threadFunc;
      newThread->stackDepth = stackDepth;
      newThread->funcParams = threadFuncParams;
      newThread->priority   = threadPriority;
      newThread->handle     = threadHandle;

      memcpy( &newThread->name[ 0 ], threadName, sizeof( Thread_t::name ) );

      /*------------------------------------------------
      The user may not care to have a handle, but the
      internal thread library processing needs the handle 
      to implement various behaviors. Create one in this case.
      ------------------------------------------------*/
      if ( !newThread->handle )
      {
        newThread->handle = new TaskHandle_t;
      }

      /*------------------------------------------------
      Immediately create the new thread if the scheduler is already running
      ------------------------------------------------*/
      if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
      {
        error = xTaskCreate( newThread->func, newThread->name, newThread->stackDepth, newThread->funcParams,
                             newThread->priority, newThread->handle );
      }

      /*------------------------------------------------
      Register the newly created thread so we can manage it.
      ------------------------------------------------*/
      if ( error == pdPASS )
      {
        sSystemThreadRegistry.push_back( newThread );
      }

      return error;
    }

    BaseType_t addThread( Thread_t &thread )
    {
      return addThread( thread.func, thread.name, thread.stackDepth, thread.funcParams, thread.priority, thread.handle );
    }

    BaseType_t signalSetupComplete()
    {
      /*------------------------------------------------
      Make sure the initialization thread actually exists
      before we go trying to use it.
      ------------------------------------------------*/
      if ( sSetupCallbacksEnabled && sInitThreadHandle )
      {
        uint32_t tmp = ~sMsgSetupResume;
        while ( tmp != sMsgSetupResume )
        {
          xTaskNotify( sInitThreadHandle, sMsgSetupComplete, eSetValueWithOverwrite );
          xTaskNotifyWait( 0u, 0u, &tmp, portMAX_DELAY );
        }
      }

      return pdPASS;
    }

    void awaitTaskMessage( const size_t taskMsg )
    {
      while ( 1 )
      {
        if ( ulTaskNotifyTake( pdTRUE, portMAX_DELAY ) == taskMsg )
        {
          break;
        }
      }
    }

#ifdef SIM
    void endScheduler()
    {
      vTaskEndScheduler();
    }
#endif

}  // namespace Chimera
