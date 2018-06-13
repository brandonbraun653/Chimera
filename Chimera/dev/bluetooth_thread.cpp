/* Thor Includes */
#include <Thor/include/threads.hpp>
#include <Thor/include/serial.hpp>

/* Project Includes */
#include "bluetooth_thread.hpp"

using namespace Thor::Threading;
using namespace Thor::Peripheral::Serial;

void bluetoothThread(void* arguments)
{
	SemaphoreHandle_t rxWakeup = xSemaphoreCreateBinary();

	uint8_t receiveBuffer[25];
	memset(receiveBuffer, 0, 25);

	/* Rev 1.0 of FC board has BT on uart5 */
	SerialClass serial(5);

	serial.begin(BaudRate::SERIAL_BAUD_9600);
	serial.setMode(SubPeripheral::TX, Modes::INTERRUPT);
	serial.setMode(SubPeripheral::RX, Modes::INTERRUPT);
	serial.attachThreadTrigger(RX_COMPLETE, &rxWakeup);

	signalThreadSetupComplete();

	TickType_t lastTimeWoken = xTaskGetTickCount();
	for (;;)
	{
		xSemaphoreTake(rxWakeup, portMAX_DELAY);

		//Simply echo whatever was sent
		if (serial.availablePackets())
		{
			size_t packetSize = serial.nextPacketSize();
			serial.readPacket(receiveBuffer, packetSize);
			serial.write(receiveBuffer, packetSize);
		}
		else
		{
			serial.write("Woke up,but no data.\r\n");
		}
		
	}
}