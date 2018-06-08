/* Thor Includes */
#include <Thor/include/threads.hpp>

/* Project Includes */
#include "flash_thread.hpp"
#include "at45db081.hpp"

using namespace Thor::Threading;

void flashThread(void* arguments)
{
	Adesto::NORFlash::AT45 flash(1);

	flash.initialize(40000000);

	volatile uint16_t statusReg = flash.readStatusRegister();
	
	signalThreadSetupComplete();
	TickType_t lastTimeWoken = xTaskGetTickCount();
	for (;;)
	{


		vTaskDelayUntil(&lastTimeWoken, pdMS_TO_TICKS(1000));
	}
}