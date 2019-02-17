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
      vTaskDelay( pdMS_TO_TICKS( ms ) );
    }

    void delayMicroseconds( uint32_t us )
    {
      TickType_t tickDelay = pdMS_TO_TICKS(us*1000) / 1000;
      vTaskDelay(tickDelay);
    }
  }  // namespace Mock
}  // namespace Chimera
