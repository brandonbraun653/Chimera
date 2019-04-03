/********************************************************************************
 *   File Name:
 *     threading.hpp
 *
 *   Description:
 *     Implements the threading features for Chimera
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_THREADING_HPP
#define CHIMERA_THREADING_HPP

/* C++ Includes */
#include <atomic>

/* Chimera Includes */
#include <Chimera/types.hpp>

#ifdef USING_FREERTOS

/* FreeRTOS Includes */
#ifdef __cplusplus
extern "C"
{
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
}
#endif /* __cplusplus */
#endif /* CHIMERA_FREERTOS */

namespace Chimera
{
  namespace Threading
  {
    /**
     *  Implements lock functionality that can simply be inherited into other classes.
     *  Only guaranteed to be thread safe if using FreeRTOS.
     */
    class Lockable
    {
    public:
      virtual Chimera::Status_t reserve( const uint32_t timeout_mS )
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }

      virtual Chimera::Status_t release( const uint32_t timeout_mS )
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }

      bool isLocked();

      Lockable()  = default;
      ~Lockable() = default;

    protected:
      void lock();

      void unlock();

    private:
      std::atomic<bool> mutex;
    };

#ifdef USING_FREERTOS

    /*------------------------------------------------
    Fully describes thread creation parameters as used in overloaded addThread
    ------------------------------------------------*/
    struct Thread_t
    {
      TaskFunction_t func;  /**< Function pointer to the thread */
      TaskHandle_t handle;  /**< FreeRTOS generated handle for reference elsewhere */
      UBaseType_t priority; /**< FreeRTOS priority number, ranging from 0 to (configMAX_PRIORITIES - 1) lowest to highest */
      void *funcParams;     /**< Thread parameters to be passed in upon creation */
      uint32_t stackDepth;  /**< Size of the thread stack, in multiples of **WORDS** (x4 bytes), ie stack of 150 == 600 bytes */
      char name[ configMAX_TASK_NAME_LEN ]; /**< User friendly name for the thread */
    };

    /**
     *  Starts the FreeRTOS scheduler and initializes all registered threads.
     *
     *  This implementation extends the basic FreeRTOS vTaskStartScheduler() function
     *  by allowing the user to control the initialzation timing. If callbacks are used, then
     *  each thread will initialize in the order registered and the next thread cannot start
     *  until the current has signaled its setup sequence is complete.
     *
     *  @see signalSetupComplete();
     *
     *	@param[in] useSetupCallbacks	Enables or disables the use of setup callbacks for proper thread initialization.
     *	@return void
     */
    void startScheduler( const bool useSetupCallbacks = true );

#ifdef SIM
    /**
     *  Kills the underlying RTOS execution.
     *
     *  @return void
     */
    void endScheduler();
#endif

    /**
     *	Registers an array of thread initialization structs with the FreeRTOS scheduler. This is
     *  intended to register a large number of threads that initialize the whole embedded system.
     *
     *  @note The scheduler must not be running otherwise the registration will fail.
     *
     *	@param[in]	threads     Array of threads that will be registered
     *	@return BaseType_t
     */
    BaseType_t addThread( const Thread_t *const threadArray, const uint8_t numThreads );

    /**
     *  Adds a new thread to the FreeRTOS kernel. If the scheduler has been started already, the
     *	correct initialization sequence will be followed. Otherwise, the thread will be suspended
     *	until startScheduler() has been called.
     *
     *	@param[in]	 func			    Function pointer to the thread to be executed
     *	@param[in]   name			    User friendly name for the thread
     *	@param[in]   stackDepth		Size of the thread stack, in multiples of **WORDS** (x4 bytes), ie stack of 150 == 600 bytes
     *	@param[in]   funcParams		Thread parameters to be passed in upon creation
     *	@param[in]   priority		  FreeRTOS priority number, ranging from 0 to (configMAX_PRIORITIES - 1) lowest to highest
     *	@param[out]  handle			  FreeRTOS generated handle for reference elsewhere
     *	@return BaseType_t
     */
    BaseType_t addThread( TaskFunction_t func, const char *name, const uint16_t stackDepth, void *const funcParams,
                          UBaseType_t priority, TaskHandle_t handle );

    /**
     *  Performs the same operation as the more verbose overload
     *
     *	@param[in]	thread	      Thread initializer data
     *	@return BaseType_t
     */
    BaseType_t addThread( const Thread_t &thread );

    /**
     *  Signal back to startScheduler() that initialization has completed and the next
     *  thread (if it exists) can be started.
     *
     *  @note This only has an effect if startScheduler() was called with initialization callbacks turned on
     *  @see startScheduler()
     *
     *	@return BaseType_t
     */
    BaseType_t signalSetupComplete();

#endif /* !CHIMERA_FREERTOS */


  }  // namespace Threading
}  // namespace Chimera


#endif
