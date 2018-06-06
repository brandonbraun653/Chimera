/* FreeRTOS Includes */
#include "FreeRTOS.h"
#include "task.h"

/* Chimera Includes */
#include <Chimera/spi.hpp>

/* Thor Includes */
#include <Thor/include/threads.hpp>

/* Project Includes */
#include "spi_thread.hpp"

//using namespace Thor::Definitions;
using namespace Chimera::SPI;
using namespace Thor::Threading;

void spiThread(void* argument)
{
	auto spi = boost::make_shared<SPIClass>(1);

	Setup spiSetup;

	spiSetup.clockFrequency = 12000000;
	spiSetup.bitOrder = MSB_FIRST;
	spiSetup.clockMode = MODE1;
	spiSetup.mode = MASTER;

	spi->begin(spiSetup);
	
	spi->setMode(SubPeripheral::TXRX, SubPeripheralMode::DMA);
	
	uint8_t data_in[5] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDD };
	uint8_t data_out[5];
	
	memset(data_out, 0, 5);
	
	signalThreadSetupComplete();
	
	TickType_t lastTimeWoken = xTaskGetTickCount();
	for (;;)
	{
		//spi->write(data_in, 5, false);
		spi->write(data_in, data_out, 5);
		vTaskDelayUntil(&lastTimeWoken, pdMS_TO_TICKS(1000));
	}
}