#ifdef DEBUG
#include "SysprogsProfiler.h"
#endif

#include <Chimera/chimera.hpp>
#include <Chimera/config.hpp>

#include <Thor/include/threads.hpp>

#include "led_thread.hpp"
#include "spi_thread.hpp"
#include "serial_thread.hpp"
#include "flash_thread.hpp"
#include "bluetooth_thread.hpp"


TaskHandle_t serialTask;
TaskHandle_t ledTask;
TaskHandle_t spiTask;
TaskHandle_t flashTask;
TaskHandle_t btTask;

//pc8

int main(void)
{
	using namespace Thor::Threading;
	using namespace Thor::Peripheral::GPIO;
	
	HAL_Init();
	ThorInit();
	ChimeraInit();

	#ifdef DEBUG
	InitializeSamplingProfiler();
	InitializeInstrumentingProfiler();
	#endif

	
	addThread(ledThread, "led", 350, NULL, 2, &ledTask);
	//addThread(serialThread, "serial", 1000, NULL, 2, &serialTask);
	//addThread(spiThread, "spi", 500, NULL, 2, &spiTask);
	//addThread(flashThread, "flash", 500, NULL, 2, &flashTask);
	addThread(bluetoothThread, "bt", 500, NULL, 2, &btTask);
	startScheduler();
	
	/* Should never reach here as scheduler should be running */
	for (;;)
	{
		
	}
}

