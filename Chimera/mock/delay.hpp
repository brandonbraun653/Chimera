/********************************************************************************
 *   File Name:
 *       mock_delay.hpp
 *
 *   Description:
 *       Mocks common delay functions used on embedded targets
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/
#ifndef CHIMERA_MOCK_DELAY_HPP
#define CHIMERA_MOCK_DELAY_HPP

#include <cstdint>

namespace Chimera
{
  namespace Mock
  {
    void SystemTickCallback();



    uint32_t millis();

    void delayMilliseconds( uint32_t ms );

    void delayMicroseconds( uint32_t us );
  }  // namespace Mock
}  // namespace Chimera

#endif
