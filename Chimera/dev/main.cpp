#ifdef DEBUG
#include "SysprogsProfiler.h"
#endif

#include <Chimera/chimera.hpp>
#include <Chimera/config.hpp>
#include <Chimera/threading.hpp>

#include "led_thread.hpp"
#include "spi_thread.hpp"

#define LED_THREAD_IDX 1
#define SPI_THREAD_IDX 2

void init(void* argument);

int main(void)
{
	using namespace Chimera::Threading;
	
	HAL_Init();
	ThorInit();
	ChimeraInit();

	#ifdef DEBUG
	InitializeSamplingProfiler();
	InitializeInstrumentingProfiler();
	#endif
	// 	Chimera::SPI::SPIClass test_spi(2);
	// 	Chimera::SPI::Setup setup;
	// 	test_spi.init(setup);
	
	TaskHandle_t initHandle;
	xTaskCreate(init, "init", 500, NULL, 1, &initHandle);
	
	if (xRegisterTaskHandle(INIT_THREAD, initHandle) != pdPASS)
	{
		for (;;)
		{
			//You failed init.
		}
	}
	
	vTaskStartScheduler();
	
	/* Should never reach here as scheduler should be running */
	for (;;)
	{
		
	}
}

void init(void* argument)
{
	using namespace Chimera::Threading;
	
	volatile BaseType_t error = pdPASS;
	TickType_t lastTimeWoken = xTaskGetTickCount();
	TaskHandle_t threadHandle;
	
	error = xTaskCreate(ledThread, "ledThread", 350, NULL, 2, &threadHandle);
	while (!ulTaskNotifyTake(pdTRUE, 0))
		vTaskDelayUntil(&lastTimeWoken, pdMS_TO_TICKS(10));
	
	if (xRegisterTaskHandle(LED_THREAD_IDX, threadHandle) != pdPASS)
	{
		for (;;)
		{
			//You failed init.
		}
	}
	
	vTaskResume(TaskHandle[LED_THREAD_IDX]);
	xRemoveTaskHandle(INIT_THREAD);
}
