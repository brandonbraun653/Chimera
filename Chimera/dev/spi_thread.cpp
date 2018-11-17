/* FreeRTOS Includes */
#include "FreeRTOS.h"
#include "task.h"

/* Chimera Includes */
#include <Chimera/spi.hpp>

/* Thor Includes */
#include <Thor/include/threads.hpp>

/* Project Includes */
#include "spi_thread.hpp"

using namespace Chimera::SPI;
using namespace Chimera;
using namespace Thor::Threading;

void spiThread(void* argument)
{
	auto spi = boost::make_shared<SPIClass>(3);

	Setup spiSetup;

	spiSetup.clockFrequency = 1000000;
    spiSetup.bitOrder = BitOrder::MSB_FIRST;
    spiSetup.clockMode = ClockMode::MODE1;
    spiSetup.mode = Mode::MASTER;

    spiSetup.CS.port = GPIO::Port::PORTC;
    spiSetup.CS.alternate = Thor::Peripheral::GPIO::NOALTERNATE;
    spiSetup.CS.number = 1;
    spiSetup.CS.mode = GPIO::Drive::OUTPUT_PUSH_PULL;
    spiSetup.CS.state = GPIO::State::HIGH;

	spi->init(spiSetup);

	spi->setPeripheralMode(SubPeripheral::TXRX, SubPeripheralMode::BLOCKING);

	uint8_t tx[5] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDD };
	uint8_t rx[5];

	memset(rx, 0, 5);

	signalThreadSetupComplete();

	TickType_t lastTimeWoken = xTaskGetTickCount();
	for (;;)
	{
    	if(spi->reserve(100) == SPI::Status::OK)
    	{
            spi->writeBytes(tx, 5, true);
        	spi->release();
    	}
		
		vTaskDelayUntil(&lastTimeWoken, pdMS_TO_TICKS(1000));
	}
}
