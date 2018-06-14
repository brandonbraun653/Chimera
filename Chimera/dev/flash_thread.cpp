/* Thor Includes */
#include <Thor/include/threads.hpp>

/* Project Includes */
#include "flash_thread.hpp"
#include "at45db081.hpp"

using namespace Thor::Threading;

#define FLASH_SPI_CHANNEL 1

//#pragma pack(push,1)
struct RandomSettingsPacked
{
	float kp;
	float ki;
	float kd;
};
//#pragma pack(pop)


//pa10/11

void flashThread(void* arguments)
{
	Adesto::NORFlash::AT45 flash(Adesto::AT45DB081E, FLASH_SPI_CHANNEL);
	flash.initialize(25000000);

	/* Rev 1.0 of FC board has USB COM port on uart(?) <PA10/PA11>*/
	//SerialClass serial(1);
	//serial.begin(BaudRate::SERIAL_BAUD_9600);
	//serial.setMode(SubPeripheral::TX, Modes::INTERRUPT);
	//serial.setMode(SubPeripheral::RX, Modes::INTERRUPT);
	
	RandomSettingsPacked packedData, memPackedData;
	packedData.kp = 3.03f;
	packedData.ki = 12.03f;
	packedData.kd = -1.04;

	memPackedData.kp = 0.0;
	memPackedData.ki = 0.0;
	memPackedData.kd = 0.0;

	//RandomSettingsUnPacked unpackedData = { 3.14f, 6.14f, -0.34234, "boooo!" };

	signalThreadSetupComplete();

	//TODO: Need to make a generic write function that operates on address/len only...not this page crap.
	//While I'm at it, I might as well make a template version that automatically calculates the length and does
	//the whole reinterpret cast thing?

	//TODO: Apparently I also need to add checks on write an read to see if the device is ready for that operation. Oops.

	volatile Adesto::Status error = flash.pageWrite(Adesto::BUFFER1, 0, 1, reinterpret_cast<uint8_t*>(&packedData), sizeof(RandomSettingsPacked));

	while (!flash.isWriteComplete());

	error = flash.directPageRead(1, 0, reinterpret_cast<uint8_t*>(&memPackedData), sizeof(RandomSettingsPacked));

	while (!flash.isReadComplete());

	if (memcmp(&packedData, &memPackedData, sizeof(RandomSettingsPacked)) != 0)
		error = Adesto::ERROR_WRITE_FAILURE;

	TickType_t lastTimeWoken = xTaskGetTickCount();
	for (;;)
	{
		//flash.bufferLoad(Adesto::BUFFER1, 15, data, 15);

		vTaskDelayUntil(&lastTimeWoken, pdMS_TO_TICKS(500));

		//flash.bufferRead(Adesto::BUFFER1, 15, output, 15);

		vTaskDelayUntil(&lastTimeWoken, pdMS_TO_TICKS(500));
	}
}