#include "serial_thread.hpp"

/* C/C++ Includes */
#include <string>

/* Boost Includes */
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/gpio.hpp>
#include <Chimera/threading.hpp>


#include <Thor/include/uart.h>

void serialThread(void* argument)
{
	using namespace Chimera::Threading;
	using namespace Thor::Peripheral::UART;

	UARTClass_sPtr uart = uart4;

	uart->begin(115200);
	uart->setBlockMode(UARTPeriph::TX);

	const char* test1 = "Hey mate.\n";
	const char* test2 = "Testing testing!\n";
	const char* test3 = "I love watermelon.\n";

	/* Inform the init task that everything has been set up */
	xTaskSendMessage(INIT_THREAD, 1u);
	vTaskSuspend(NULL);
	taskYIELD();


	uart->setDMAMode(UARTPeriph::TX);
	TickType_t lastTimeWoken = xTaskGetTickCount();
	for (;;)
	{
		uart->write(test1);
		uart->write(test2);
		uart->write(test3);

		vTaskDelayUntil(&lastTimeWoken, pdMS_TO_TICKS(500));
	}
}