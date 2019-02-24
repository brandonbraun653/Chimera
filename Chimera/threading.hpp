#pragma once
#ifndef CHIMERA_THREADING_HPP
#define CHIMERA_THREADING_HPP

/* C++ Includes */
#include <atomic>

/* Chimera Includes */
#include <Chimera/chimera.hpp>

namespace Chimera
{
  namespace Threading
  {
    class Lockable
    {
    public:
      virtual bool reserve( const uint32_t timeout_mS ) noexcept;

      virtual bool release( const uint32_t timeout_mS ) noexcept;

      bool isLocked();

      Lockable()  = default;
      ~Lockable() = default;

    protected:
      void lock();

      void unlock();

    private:
      std::atomic<bool> mutex;
    };
  }  // namespace Threading
}  // namespace Chimera


#ifdef CHIMERA_FREERTOS

#ifdef __cplusplus
extern "C"
{
#endif

/* FreeRTOS Includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#ifdef __cplusplus
}
#endif

namespace Chimera
{
  namespace Threading
  {
    const uint8_t maxThreads = 15;                  /**< Maximum number of threads */
    const uint32_t threadInitCheckDelay_ms = 10;    /**< Delay before checking if thread initialization is complete */
    const uint32_t maxThreadInitTimeout_ms = 10000; /**< Max time to wait for thread init sequence to complete */

    /** Fully describes thread creation parameters as used in overloaded addThread */
    struct Thread_t
    {
      TaskFunction_t func;  /**< Function pointer to the thread */
      const char *name;     /**< User friendly name for the thread */
      uint16_t stackDepth;  /**< Size of the thread stack, in multiples of **WORDS** (x4 bytes), ie stack of 150 == 600 bytes */
      void *funcParams;     /**< Thread parameters to be passed in upon creation */
      UBaseType_t priority; /**< FreeRTOS priority number, ranging from 0 to (configMAX_PRIORITIES - 1) lowest to highest */
      TaskHandle_t handle;  /**< FreeRTOS generated handle for reference elsewhere */
    };

    /**
     *  Starts the FreeRTOS scheduler and initializes execution of all registered threads. This implementation extends the basic
     *  FreeRTOS vTaskStartScheduler() function by automatically supporting user setup code in each thread.
     *
     *	@param[in] useSetupCallbacks	Enables or disables the use of setup callbacks for proper thread initialization.
     *	                              Defaults to true.
     *	@return void
     */
    void startScheduler( bool useSetupCallbacks = true );

#ifdef SIM
    /**
     *  Kills the underlying RTOS execution.
     *
     *  @return void
     */
    void endScheduler();
#endif

    /** Adds a new thread to the FreeRTOS kernel. If the scheduler has been started already, the
     *	correct initialization sequence will be followed. Otherwise, the thread will be suspended
     *	until startScheduler() has been called.
     *
     *	@param[in]	 func			Function pointer to the thread to be executed
     *	@param[in]   name			User friendly name for the thread
     *	@param[in]   stackDepth		Size of the thread stack, in multiples of **WORDS** (x4 bytes), ie stack of 150 == 600 bytes
     *	@param[in]   funcParams		Thread parameters to be passed in upon creation
     *	@param[in]   priority		FreeRTOS priority number, ranging from 0 to (configMAX_PRIORITIES - 1) lowest to highest
     *	@param[out]  handle			FreeRTOS generated handle for reference elsewhere
     *
     *	@return pdPASS if successful, pdFAIL if not
     **/
    BaseType_t addThread( TaskFunction_t func, const char *name, const uint16_t stackDepth, void *const funcParams,
                    UBaseType_t priority, TaskHandle_t handle );

    /** Performs the same operation as the more verbose version, but uses the more compact Thread_t struct to pass in parameters
     *	@param[in]	thread	Thread parameters from the Thread_t struct
     *
     *	@return pdPASS if successful, pdFAIL if not
     **/
    BaseType_t addThread( Thread_t &thread );

    /** Safely removes a thread from existence
     *	@param[in]	task	The handle of the thread to be deleted
     *	@return void
     **/
    extern void deleteThread( TaskHandle_t task );

    /** During creation of a thread, there is usually some setup code and an infinite loop. This function is a way to halt
     *	execution of a new thread after the setup code and wait for other threads to initialize before continuing. This assumes
     *	that the scheduler was just started. If already running, the halting is ignored and return pdFAIL because the internal
     *	initialization thread has been destroyed.
     *
     *	@return pdPASS if successful, pdFAIL if not
     **/
    extern BaseType_t signalThreadSetupComplete();

    /** Send a simple message to a thread if the task handle is known
     *	@param[in] task	Handle of the thread to send a message to
     *	@param[in] msg	The message
     *
     *	@return pdPASS if successful, pdFAIL if not
     **/
    extern BaseType_t sendMessageAndWait( TaskHandle_t task, const uint32_t msg );

    extern BaseType_t sendMessage( TaskHandle_t task, const uint32_t msg );


    /** Allows for a fixed number of semaphores to be taken with timeout. DO NOT USE in ISR.
     *  Note this can cause a worst case blocking time (in ticks) of xTicksToWait * numTake
     *  @param[in]  xSemaphore      The semaphore to be taken from
     *  @param[in]  xTicksToWait    How many ticks to wait for each take
     *  @param[in]  numTake         Number of semaphores expected to take before returning
     *  @return                     pdPASS if all taken successfully, pdFAIL if any timeout
     */
    extern BaseType_t xSemaphoreTakeMultiple( SemaphoreHandle_t xSemaphore, TickType_t xTicksToWait, size_t numTake );
  }  // namespace Threading
}  // namespace Chimera
#endif /* !CHIMERA_FREERTOS */


#endif
