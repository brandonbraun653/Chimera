/* Thor Includes */
#include <Thor/include/threads.hpp>

/* Project Includes */
#include "flash_thread.hpp"
#include "at45db081.hpp"

using namespace Thor::Threading;

#define FLASH_SPI_CHANNEL 1

#pragma pack(push,1)
struct RandomSettingsPacked
{
	float kp;
	float ki;
	float kd;
	//char dummy[256];
};
#pragma pack(pop)


//pa10/11

void flashThread(void* arguments)
{
	Adesto::NORFlash::AT45 flash(Adesto::AT45DB081E, FLASH_SPI_CHANNEL);
	flash.initialize(1000000);

	/* Rev 1.0 of FC board has USB COM port on uart(?) <PA10/PA11>*/
	//SerialClass serial(1);
	//serial.begin(BaudRate::SERIAL_BAUD_9600);
	//serial.setMode(SubPeripheral::TX, Modes::INTERRUPT);
	//serial.setMode(SubPeripheral::RX, Modes::INTERRUPT);
	
	RandomSettingsPacked packedData, dummyData;
	packedData.kp = 3.03f;
	packedData.ki = 12.03f;
	packedData.kd = -1.04;

	dummyData.kp = 0.0f;
	dummyData.ki = 0.0f;
	dummyData.kd = 0.0f;

	//memset(packedData.dummy, 35, 256);
	//memset(dummyData.dummy, 0, 256);
	volatile Adesto::Status error;

	//RandomSettingsUnPacked unpackedData = { 3.14f, 6.14f, -0.34234, "boooo!" };

	signalThreadSetupComplete();

	//error = flash.pageWrite(Adesto::BUFFER1, 0, 1, reinterpret_cast<uint8_t*>(&packedData), sizeof(RandomSettingsPacked));

	//while (!flash.isWriteComplete());

	//error = flash.directPageRead(1, 0, reinterpret_cast<uint8_t*>(&memPackedData), sizeof(RandomSettingsPacked));

	//while (!flash.isReadComplete());

	//if (memcmp(&packedData, &memPackedData, sizeof(RandomSettingsPacked)) != 0)
	//	error = Adesto::ERROR_WRITE_FAILURE;


	//error = flash.write(0, reinterpret_cast<uint8_t*>(&packedData), sizeof(RandomSettingsPacked));
	//error = flash.read(0, reinterpret_cast<uint8_t*>(&dummyData), sizeof(RandomSettingsPacked));

	error = flash.write(0, packedData);
	error = flash.read(0, dummyData);

	//flash.read

	if (memcmp(&packedData, &dummyData, sizeof(RandomSettingsPacked)) != 0)
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