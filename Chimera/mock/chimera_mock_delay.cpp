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

namespace Chimera
{
  namespace Mock
  {
    uint32_t millis()
    {
      // Call the freertos tick thing
      return 0u;
    }

    void delayMilliseconds( uint32_t ms )
    {
      // Eventually call into the freertos windows version
    }

    void delayMicroseconds( uint32_t us )
    {
      // Do nothing?
    }
  }  // namespace Mock
}  // namespace Chimera
