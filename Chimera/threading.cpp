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

namespace Chimera
{
  namespace Threading
  {
    Lockable::Lockable()
    {
#if defined( USING_FREERTOS )
      recursive_mutex = xSemaphoreCreateRecursiveMutex();
#else
      // Handle standard std::recursive_mutex
#endif
    }

    Chimera::Status_t Lockable::reserve( const uint32_t timeout_mS )
    {
      Chimera::Status_t error = Chimera::CommonStatusCodes::OK;

#if defined( USING_FREERTOS )
      if ( xSemaphoreTake( recursive_mutex, pdMS_TO_TICKS( timeout_mS ) ) != pdPASS )
      {
        error = Chimera::CommonStatusCodes::FAIL;
      }
#else
      // Handle standard std::recursive_mutex
#endif

      return error;
    }

    Chimera::Status_t Lockable::release()
    {
      Chimera::Status_t error = Chimera::CommonStatusCodes::OK;

#if defined( USING_FREERTOS )
      if ( xSemaphoreGive( recursive_mutex ) != pdPASS )
      {
        error = Chimera::CommonStatusCodes::FAIL;
      }
#else
      // Handle standard std::recursive_mutex
#endif

      return error;
    }

#ifdef USING_FREERTOS
    /*------------------------------------------------
    These bytes were generated from atmospheric noise, so they
    are pretty random I guess. https://www.random.org/bytes/
    ------------------------------------------------*/
    static constexpr uint32_t MSG_SETUP_COMPLETE = 0xbf931c86;
    static constexpr uint32_t MSG_SETUP_RESUME   = 0x2e526078;

    static std::vector<Thread_t *> systemThreads;
    static TaskHandle_t INIT_THREAD;
    static bool setupCallbacksEnabled = false;

    /**
     *  Implements a simple timeout while waiting for a newly created thread to
     *  complete its initialization sequence.
     *
     *  @return BaseType_t
     */
    static BaseType_t threadInitTimeout()
    {
      BaseType_t error = pdPASS;

      /*------------------------------------------------
      Immediately return pdPASS if we aren't using setup callbacks
      ------------------------------------------------*/
      if ( setupCallbacksEnabled )
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
     *  Private thread that performs the startup sequence on all threads registered
     *  before startScheduler() was called
     *
     *  @return void
     */
    static void initThread( void *arguments )
    {
      BaseType_t error = pdPASS;
      Thread_t *thread;

      /*------------------------------------------------
      Initialize all threads registered in the system
      ------------------------------------------------*/
      for ( size_t i = 0; i < systemThreads.size(); i++ )
      {
        thread = systemThreads[ i ];
        error  = xTaskCreate( thread->func, thread->name, thread->stackDepth, thread->funcParams, thread->priority, &thread->handle );

        /*------------------------------------------------
        If you get stuck here, the current thread tried to allocate
        more memory than what is left in the heap.
        ------------------------------------------------*/
        if ( error == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY )
        {
          /* Uncomment this to figure out how much stack size is remaining */
          //volatile size_t bytesRemaining __attribute( ( unused ) ) = xPortGetFreeHeapSize();
          while ( 1 )
          {
            /* Kill time while we wait for the watchdog to reset us */
          }
        }

        /*------------------------------------------------
        Wait for the setup to complete. If you get stuck here, it's probably
        because you didn't use the signalSetupComplete() callback...
        ------------------------------------------------*/
        if ( threadInitTimeout() != pdPASS )
        {
          vTaskSuspendAll();
          while ( 1 )
          {
            /* Kill time while we wait for the watchdog to reset us */
          }
        }
      }

      /*------------------------------------------------
      Resume all threads registered in the system
      ------------------------------------------------*/
      if ( setupCallbacksEnabled )
      {
        for ( size_t i = 0; i < systemThreads.size(); i++ )
        {
          if ( systemThreads[ i ]->handle )
          {
            xTaskNotify( systemThreads[ i ]->handle, MSG_SETUP_RESUME, eSetValueWithOverwrite );
          }
          else
          {
            while ( 1 )
            {
              /* Chill out while the watchdog resets us...this really shouldn't be happening at this point */
            }
          }
        }
      }

      /*------------------------------------------------
      Cleanly exit this initialization thread
      ------------------------------------------------*/
      INIT_THREAD = nullptr;
      setupCallbacksEnabled = false;
      vTaskDelete( NULL );
    }

    void startScheduler( const bool useSetupCallbacks )
    {
      setupCallbacksEnabled = useSetupCallbacks;
      xTaskCreate( initThread, "threading_init", 500, NULL, 1, &INIT_THREAD );
      vTaskStartScheduler();
    }

    BaseType_t addThread( Thread_t *const threadArray, const uint8_t numThreads )
    {
      for ( uint8_t x = 0; x < numThreads; x++ )
      {
        systemThreads.push_back( &threadArray[ x ] );
      }

      return pdPASS;
    }

    BaseType_t addThread( TaskFunction_t threadFunc, const char *threadName, const uint16_t stackDepth,
                          void *const threadFuncParams, UBaseType_t threadPriority, TaskHandle_t threadHandle )
    {
      BaseType_t error = pdPASS;

      Thread_t *newThread = new Thread_t;
      newThread->func       = threadFunc;
      newThread->stackDepth = stackDepth;
      newThread->funcParams = threadFuncParams;
      newThread->priority   = threadPriority;
      newThread->handle     = threadHandle;
      memcpy( &newThread->name[0], threadName, sizeof( Thread_t::name ) );

      /*------------------------------------------------
      Immediately create the new thread if the scheduler is already running
      ------------------------------------------------*/
      if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
      {
        error = xTaskCreate( newThread->func, newThread->name, newThread->stackDepth, newThread->funcParams, newThread->priority,
                             &newThread->handle );
      }

      /*------------------------------------------------
      Regardless of if the scheduler is running or not, it we have
      success, then register the thread with the system.
      ------------------------------------------------*/
      if ( error == pdPASS )
      {
        systemThreads.push_back( newThread );
      }

      return error;
    }

    BaseType_t addThread( Thread_t &thread )
    {
      return addThread( thread.func, thread.name, thread.stackDepth, thread.funcParams, thread.priority, thread.handle );
    }

    BaseType_t signalSetupComplete()
    {
      if ( setupCallbacksEnabled && INIT_THREAD )
      {
        uint32_t tmp = ~MSG_SETUP_RESUME;
        while ( tmp != MSG_SETUP_RESUME )
        {
          xTaskNotify( INIT_THREAD, MSG_SETUP_COMPLETE, eSetValueWithOverwrite );
          xTaskNotifyWait( 0u, 0u, &tmp, portMAX_DELAY );
        }
      }

      return pdPASS;
    }

#ifdef SIM
    void endScheduler()
    {
      vTaskEndScheduler();
    }
#endif

#endif  /* USING_FREERTOS */

  }  // namespace Threading
}  // namespace Chimera
