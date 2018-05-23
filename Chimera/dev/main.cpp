#ifdef DEBUG
#include "SysprogsProfiler.h"
#endif

#include <Chimera/chimera.hpp>
#include <Chimera/config.hpp>
#include <Chimera/threading.hpp>

#include "led_thread.hpp"
#include "spi_thread.hpp"
#include "serial_thread.hpp"

#include <Thor/include/gpio.hpp>

#define LED_THREAD_IDX 1
#define SPI_THREAD_IDX 2
#define SER_THREAD_IDX 3

/*
Blue Led: PB7
Red Led: PB14
Green Led: PB0
*/

void init(void* argument);

int main(void)
{
	using namespace Chimera::Threading;
	using namespace Thor::Peripheral::GPIO;
	
	HAL_Init();
	ThorInit();
	ChimeraInit();

	#ifdef DEBUG
	InitializeSamplingProfiler();
	InitializeInstrumentingProfiler();
	#endif

	
	TaskHandle_t initHandle;
	xTaskCreate(init, "init", 500, NULL, 1, &initHandle);
	xRegisterTaskHandle(INIT_THREAD, initHandle);
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
	
	/* Create the LED Thread  */
	error = xTaskCreate(ledThread, "ledThread", 350, NULL, 2, &threadHandle);
	while (!ulTaskNotifyTake(pdTRUE, 0))
		vTaskDelayUntil(&lastTimeWoken, pdMS_TO_TICKS(10));
	
	xRegisterTaskHandle(LED_THREAD_IDX, threadHandle);

	/* Create the Serial Thread */
	error = xTaskCreate(serialThread, "serialThread", 1000, NULL, 2, &threadHandle);
	while (!ulTaskNotifyTake(pdTRUE, 0))
		vTaskDelayUntil(&lastTimeWoken, pdMS_TO_TICKS(10));

	xRegisterTaskHandle(SER_THREAD_IDX, threadHandle);

	
	vTaskResume(TaskHandle[LED_THREAD_IDX]);
	vTaskResume(TaskHandle[SER_THREAD_IDX]);
	xRemoveTaskHandle(INIT_THREAD);
}
