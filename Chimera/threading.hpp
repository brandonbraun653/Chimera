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
		const uint32_t INIT_THREAD = 0;
		const uint32_t MAX_THREADS = 10;
		extern TaskHandle_t TaskHandle[];
		
		extern BaseType_t xRegisterTaskHandle(const uint32_t idx, TaskHandle_t handle);
		extern BaseType_t xRemoveTaskHandle(const uint32_t idx);
		extern BaseType_t xTaskSendMessage(const uint32_t idx, const uint32_t);
		extern BaseType_t xTaskSendMessageFromISR(const uint32_t idx, const uint32_t);
	}
}
#endif /* !CHIMERA_FREERTOS */
#endif 