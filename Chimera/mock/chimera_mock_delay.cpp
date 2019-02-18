/********************************************************************************
 *   File Name:
 *       chimera_mock_delay.cpp
 *
 *   Description:
 *       Mocks the delay (asynchronous) functions supported by Chimera
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#include <Chimera/mock/delay.hpp>
#include <Chimera/threading.hpp>

/* Boost Includes */
#include <boost/chrono.hpp>
#include <boost/thread.hpp>

static volatile uint32_t system_tick = 0;

namespace Chimera
{
  namespace Mock
  {
    void SystemTickCallback()
    {
      system_tick++;
    }

    uint32_t millis()
    {
      return system_tick;
    }

    void delayMilliseconds( uint32_t ms )
    {
      // TODO: I'm going to need a way to determine if this was called from a FreeRTOS thread 
      //       or the simulation environment. There is a shot both conditions could be met
      //       and code is exercised in two places. Perhaps I should add an optional sim only
      //       parameter that defaults to signaling a freertos task, but a normal thread can
      //       override the thing if needed.

      vTaskDelay( pdMS_TO_TICKS( ms ) );

//       TaskHandle_t hndl = xTaskGetCurrentTaskHandle();
//       std::string name  = pcTaskGetName(hndl);
// 
//       if( name.compare("IDLE") != 0 )
//       {
//         vTaskDelay( pdMS_TO_TICKS( ms ) );
//       }
//       else
//       {
//         boost::chrono::milliseconds time(ms);
//         boost::this_thread::sleep_for(time);
//       }
    }

    void delayMicroseconds( uint32_t us )
    {
      TickType_t tickDelay = pdMS_TO_TICKS(us*1000) / 1000;
      vTaskDelay(tickDelay);
    }
  }  // namespace Mock
}  // namespace Chimera
