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

namespace Chimera::Threading 
{
    Lockable::Lockable()
    {
      recursive_mutex = xSemaphoreCreateRecursiveMutex();
    }

    Chimera::Status_t Lockable::lock( const uint32_t timeout_mS )
    {
      Chimera::Status_t error = Chimera::CommonStatusCodes::OK;

      if ( xSemaphoreTake( recursive_mutex, pdMS_TO_TICKS( timeout_mS ) ) != pdPASS )
      {
        error = Chimera::CommonStatusCodes::FAIL;
      }

      return error;
    }

    Chimera::Status_t Lockable::unlock()
    {
      Chimera::Status_t error = Chimera::CommonStatusCodes::OK;

      if ( xSemaphoreGive( recursive_mutex ) != pdPASS )
      {
        error = Chimera::CommonStatusCodes::FAIL;
      }

      return error;
    }


    /*------------------------------------------------
    These bytes were generated from atmospheric noise, so they
    are pretty random I guess. https://www.random.org/bytes/
    ------------------------------------------------*/
    static constexpr uint32_t MSG_SETUP_COMPLETE = 0xbf931c86;
    static constexpr uint32_t MSG_SETUP_RESUME   = 0x2e526078;

    static std::vector<Thread_t *> s_systemThreads;
    static TaskHandle_t s_init_thread;
    static bool s_setupCallbacksEnabled = false;

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
      if ( s_setupCallbacksEnabled )
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
      Initialize all threads internally registered in the system
      ------------------------------------------------*/
      for ( size_t i = 0; i < s_systemThreads.size(); i++ )
      {
        thread = s_systemThreads[ i ];
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
      if ( s_setupCallbacksEnabled )
      {
        for ( size_t i = 0; i < s_systemThreads.size(); i++ )
        {
          if ( s_systemThreads[ i ]->handle )
          {
            xTaskNotify( *s_systemThreads[ i ]->handle, MSG_SETUP_RESUME, eSetValueWithOverwrite );
          }
        }
      }

      /*------------------------------------------------
      Cleanly exit this initialization thread. It's not 
      needed any more.
      ------------------------------------------------*/
      s_init_thread           = nullptr;
      s_setupCallbacksEnabled = false;
      vTaskDelete( NULL );
    }

    void startScheduler( const bool useSetupCallbacks )
    {
      s_setupCallbacksEnabled = useSetupCallbacks;
      xTaskCreate( initThread, "threading_init", 500, NULL, 1, &s_init_thread );
      vTaskStartScheduler();
    }

    BaseType_t addThread( Thread_t *const threadArray, const uint8_t numThreads )
    {
      for ( uint8_t x = 0; x < numThreads; x++ )
      {
        s_systemThreads.push_back( &threadArray[ x ] );
      }

      return pdPASS;
    }

    BaseType_t addThread( TaskFunction_t threadFunc, const char *threadName, const uint16_t stackDepth,
                          void *const threadFuncParams, UBaseType_t threadPriority, TaskHandle_t *const threadHandle )
    {
      /*------------------------------------------------
      Default to pass so the thread registration will take place later
      if the scheduler isn't running yet.
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
      Immediately create the new thread if the scheduler is already running
      ------------------------------------------------*/
      if ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
      {
        error = xTaskCreate( newThread->func, newThread->name, newThread->stackDepth, newThread->funcParams,
                             newThread->priority, threadHandle );
      }

      /*------------------------------------------------
      Register the newly created thread so we can manage it.
      ------------------------------------------------*/
      if ( error == pdPASS )
      {
        s_systemThreads.push_back( newThread );
      }

      return error;
    }

    BaseType_t addThread( Thread_t &thread )
    {
      return addThread( thread.func, thread.name, thread.stackDepth, thread.funcParams, thread.priority, thread.handle );
    }

    BaseType_t signalSetupComplete()
    {
      if ( s_setupCallbacksEnabled && s_init_thread )
      {
        uint32_t tmp = ~MSG_SETUP_RESUME;
        while ( tmp != MSG_SETUP_RESUME )
        {
          xTaskNotify( s_init_thread, MSG_SETUP_COMPLETE, eSetValueWithOverwrite );
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
