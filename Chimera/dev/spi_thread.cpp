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
using namespace Chimera::GPIO;
using namespace Chimera;
using namespace Thor::Threading;

void spiThread(void* argument)
{
    uint8_t tx[5] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDD };

	auto spi = boost::make_shared<SPIClass>(3);

	Setup spiSetup;

	spiSetup.clockFrequency = 500000;
    spiSetup.bitOrder = BitOrder::MSB_FIRST;
    spiSetup.clockMode = ClockMode::MODE1;
    spiSetup.mode = Mode::SLAVE;

    if(spiSetup.mode == Mode::MASTER)
    {
        spiSetup.CS.pin = 15;
        spiSetup.CS.port = GPIO::Port::PORTA;
        spiSetup.CS.alternate = Thor::Peripheral::GPIO::NOALTERNATE;
        spiSetup.CS.mode = GPIO::Drive::OUTPUT_PUSH_PULL;
        spiSetup.CS.state = GPIO::State::HIGH;

        spi->setChipSelectControlMode(ChipSelectMode::MANUAL);
    }

	spi->init(spiSetup);
	spi->setPeripheralMode(SubPeripheral::TXRX, SubPeripheralMode::BLOCKING);

	signalThreadSetupComplete();

	TickType_t lastTimeWoken = xTaskGetTickCount();
	for (;;)
	{

        if(spiSetup.mode == Mode::MASTER)
        {
            if(spi->reserve(100) == SPI::Status::OK)
    	    {
                spi->setChipSelect(State::LOW);
                spi->writeBytes(tx, 5, true, true);
    	    }
        }
        else
        {

        }


		vTaskDelayUntil(&lastTimeWoken, pdMS_TO_TICKS(1000));
	}
}
