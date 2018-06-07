#include "led_thread.hpp"

/* C/C++ Includes */

/* Boost Includes */
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
//#include <Chimera/gpio.hpp>
//#include <Chimera/threading.hpp>

#include <Thor/include/definitions.hpp>
#include <Thor/include/gpio.hpp>
#include <Thor/include/nucleo.hpp>
#include <Thor/include/threads.hpp>

void ledThread(void* argument)
{
	//using namespace Chimera::Threading;
	using namespace Thor::Threading;
	using namespace Thor::Peripheral::GPIO;
	using namespace Thor::Definitions::GPIO;
	
	//GPIOClass green_led(Thor::Nucleo::GREEN_LED_PORT, Thor::Nucleo::GREEN_LED_PIN);
	GPIOClass blue_led(GPIOC, PIN_8);
	
	//green_led.mode(OUTPUT_PP);
	blue_led.mode(OUTPUT_PP);

	//green_led.write(HIGH);

	/* Inform the init task that everything has been set up */
	signalThreadSetupComplete();
	
	TickType_t lastTimeWoken = xTaskGetTickCount();
	for (;;)
	{
		//green_led.toggle();
		blue_led.toggle();
		vTaskDelayUntil(&lastTimeWoken, pdMS_TO_TICKS(500));
	}
}