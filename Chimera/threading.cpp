#include <Chimera/threading.hpp>

#ifdef CHIMERA_FREERTOS

namespace Chimera
{
	namespace Threading
	{
		TaskHandle_t TaskHandle[MAX_THREADS];
		
		/* Registers a TaskHandle to Chimera if the requested idx is free */
		BaseType_t xRegisterTaskHandle(const uint32_t idx, TaskHandle_t handle)
		{
			if ((idx < MAX_THREADS) && !TaskHandle[idx])
			{
				TaskHandle[idx] = handle;
				return pdPASS;
			}
			else
				return pdFAIL;
		}
		
		/* DANGEROUS: Completely destroys all references to the thread at 'idx' regardless of 
		 * which task this function is called from */
		BaseType_t xRemoveTaskHandle(const uint32_t idx)
		{
			if (idx < MAX_THREADS)
			{
				/* Removes FreeRTOS log of the task's existence. Fall through silently
				 * if the task is already empty. */
				if (TaskHandle[idx])
				{
					vTaskDelete(TaskHandle[idx]); 
				}
				
				/* Removes personal log of the task's existence */
				TaskHandle[idx] = (void*)0; 
				
				return pdPASS;
			}
			else
			{
				/* You gave an invalid handle index */
				#if CHIMERA_RTOS_FREEZE_ON_FAIL
				for(;;) {}
				#endif 
				
				return pdFAIL;
			}
		}
		
		BaseType_t xTaskSendMessage(const uint32_t idx, uint32_t msg)
		{
			if ((idx < MAX_THREADS) && TaskHandle[idx])
				return xTaskNotify(TaskHandle[idx], msg, eSetValueWithOverwrite);
			else
				return pdFAIL;
		}

		BaseType_t xTaskSendMessageFromISR(const uint32_t idx, uint32_t msg)
		{
			if ((idx < MAX_THREADS) && TaskHandle[idx])
				return xTaskNotifyFromISR(TaskHandle[idx], msg, eSetValueWithOverwrite, NULL);
			else
				return pdFAIL;
		}
	}
}

#endif 