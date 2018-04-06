#include "led_thread.hpp"

/* C/C++ Includes */

/* Boost Includes */
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/gpio.hpp>
#include <Chimera/threading.hpp>

void ledThread(void* argument)
{
	using namespace Chimera::Threading;
	using namespace Chimera::GPIO;
	
	GPIOClass blue_led(PORTB, 7);
	
	blue_led.mode(OUTPUT);
	blue_led.write(HIGH);

	/* Inform the init task that everything has been set up */
	xTaskSendMessage(INIT_THREAD, 1u);
	vTaskSuspend(NULL);
	taskYIELD();
	
	TickType_t lastTimeWoken = xTaskGetTickCount();
	for (;;)
	{
		blue_led.toggle();
		vTaskDelayUntil(&lastTimeWoken, pdMS_TO_TICKS(500));
	}
}