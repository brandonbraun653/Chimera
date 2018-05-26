#include "serial_thread.hpp"

/* C/C++ Includes */
#include <string>

/* Boost Includes */
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/threading.hpp>
#include <Chimera/gpio.hpp>
#include <Chimera/serial.hpp>


/* Thor Includes */
#include <Thor/include/definitions.hpp>
#include <Thor/include/uart.hpp>


using namespace Thor::Definitions::Serial;

void serialThread(void* argument)
{
	using namespace Chimera::Threading;
	using namespace Thor::Peripheral::UART;

	UARTClass_sPtr uart = UARTClass::create(4);

	uart->begin(SERIAL_BAUD_115200);
	uart->setMode(TX, BLOCKING);

	const char* test1 = "Hey mate.\r\n";
	const char* test2 = "Testing testing!\r\n";
	const char* test3 = "I love watermelon.\r\n";

	/* Inform the init task that everything has been set up */
	xTaskSendMessage(INIT_THREAD, 1u);
	vTaskSuspend(NULL);
	taskYIELD();


	uart->setMode(TX, DMA);
	TickType_t lastTimeWoken = xTaskGetTickCount();
	for (;;)
	{
		uart->write(test1);
		uart->write(test2);
		uart->write(test3);

		vTaskDelayUntil(&lastTimeWoken, pdMS_TO_TICKS(500));
	}
}