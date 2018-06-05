/* FreeRTOS Includes */
#include "FreeRTOS.h"
#include "task.h"

/* Thor Includes */
#include <Thor/include/definitions.hpp>
#include <Thor/include/spi.hpp>
#include <Thor/include/threads.hpp>

/* Project Includes */
#include "spi_thread.hpp"

using namespace Thor::Definitions;
using namespace Thor::Threading;

void spiThread(void* argument)
{
	auto spi = Thor::Peripheral::SPI::SPIClass::create(1);
	
	spi->begin();
	
	spi->setMode(SubPeripheral::TXRX, Modes::DMA);
	
	uint8_t data_in[5] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDD };
	uint8_t data_out[5];
	
	memset(data_out, 0, 5);
	
	signalThreadSetupComplete();
	
	TickType_t lastTimeWoken = xTaskGetTickCount();
	for (;;)
	{
		spi->write(data_in, 5, false);
		spi->write(data_in, data_out, 5);
		vTaskDelayUntil(&lastTimeWoken, pdMS_TO_TICKS(1000));
	}
}