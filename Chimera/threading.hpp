#pragma once
#ifndef CHIMERA_THREADING_HPP
#define CHIMERA_THREADING_HPP

/* Chimera Includes */
#include <Chimera/chimera.hpp>

#ifdef CHIMERA_FREERTOS
/* FreeRTOS Includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

namespace Chimera
{
	namespace Threading
	{
		const uint8_t maxThreads = 15;					/**< Maximum number of threads */
		const uint32_t threadInitCheckDelay_ms = 10;	/**< How long to wait during thread initialization before polling to check init complete */
		const uint32_t maxThreadInitTimeout_ms = 1000;  /**< Max time to wait for thread init sequence to complete */

		/** Fully describes thread creation parameters as used in overloaded addThread */
		struct Thread_t
		{
			TaskFunction_t func;	/**< Function pointer to the thread */
			const char* name;		/**< User friendly name for the thread */
			uint16_t stackDepth;	/**< Size of the thread stack, in multiples of **WORDS** (x4 bytes), ie stack of 150 == 600 bytes */
			void* funcParams;		/**< Thread parameters to be passed in upon creation */
			UBaseType_t priority;	/**< FreeRTOS priority number, ranging from 0 to (configMAX_PRIORITIES - 1) lowest to highest */
			TaskHandle_t* handle;	/**< FreeRTOS generated handle for reference elsewhere */
		};

		/** Starts the FreeRTOS scheduler and initializes execution of all registered threads. This implementation extends the basic
		*  FreeRTOS vTaskStartScheduler() function by automatically supporting user setup code in each thread.
		*
		*	@param[in] useSetupCallbacks	Enables or disables the use of setup callbacks for proper thread initialization. Defaults to true.
		*	@return void
		**/
		extern void startScheduler(bool useSetupCallbacks = true);

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
		extern BaseType_t addThread(TaskFunction_t func, const char* name, const uint16_t stackDepth, void* const funcParams,
			UBaseType_t priority, TaskHandle_t* const handle);

		/** Performs the same operation as the more verbose version, but uses the more compact Thread_t struct to pass in parameters
		*	@param[in]	thread	Thread parameters from the Thread_t struct
		*
		*	@return pdPASS if successful, pdFAIL if not
		**/
		extern BaseType_t addThread(Thread_t& thread);

		/** Safely removes a thread from existence
		*	@param[in]	task	The handle of the thread to be deleted
		*	@return void
		**/
		extern void deleteThread(TaskHandle_t task);

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
		extern BaseType_t sendMessageAndWait(TaskHandle_t task, const uint32_t msg);
	}
}
#endif /* !CHIMERA_FREERTOS */
#endif 