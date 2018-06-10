/* Thor Includes */
#include <Thor/include/threads.hpp>

/* Project Includes */
#include "flash_thread.hpp"
#include "at45db081.hpp"

using namespace Thor::Threading;

void flashThread(void* arguments)
{
	Adesto::NORFlash::AT45 flash(Adesto::AT45DB081E, 1);

	flash.initialize(40000000);

	//flash.test();

	//volatile uint16_t statusReg = flash.readStatusRegister();
	
	//uint8_t data[256];		
	//memset(data, 0xFF, 256);
	//uint8_t output[256];	
	//memset(output, 0x00, 256);

	uint8_t data[512];
	memset(data, 0x00, 512);

	

	signalThreadSetupComplete();

	//flash.bufferWrite(Adesto::BUFFER1, 15, data, 15);
	//flash.bufferRead(Adesto::BUFFER1, 15, output, 15);

	//flash.directPageRead(0, 0, output, 256);

	flash.continuousRead(0, 0, data, 512);

	TickType_t lastTimeWoken = xTaskGetTickCount();
	for (;;)
	{


		vTaskDelayUntil(&lastTimeWoken, pdMS_TO_TICKS(1000));
	}
}