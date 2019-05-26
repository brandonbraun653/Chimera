/********************************************************************************
 *   File Name:
 *       chimera_mock_delay.cpp
 *
 *   Description:
 *       Mocks the delay (asynchronous) functions supported by Chimera
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Chimera Includes */
#include <mock/Chimera/delay.hpp>
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
    }

    void delayMicroseconds( uint32_t us )
    {
    }
  }  // namespace Mock
}  // namespace Chimera
