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
	auto spi = boost::make_shared<SPIClass>(3);

	Setup spiSetup;

	spiSetup.clockFrequency = 1000000;
    spiSetup.bitOrder = BitOrder::MSB_FIRST;
    spiSetup.clockMode = ClockMode::MODE1;
    spiSetup.mode = Mode::MASTER;

	spi->init(spiSetup);

	spi->setPeripheralMode(SubPeripheral::TXRX, SubPeripheralMode::BLOCKING);

	uint8_t tx[5] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDD };
	uint8_t rx[5];

	memset(rx, 0, 5);

	signalThreadSetupComplete();

	TickType_t lastTimeWoken = xTaskGetTickCount();
	for (;;)
	{
		spi->writeBytes(tx, 5, false);
		//spi->readWriteBytes(tx, rx, 5);
		vTaskDelayUntil(&lastTimeWoken, pdMS_TO_TICKS(1000));
	}
}
