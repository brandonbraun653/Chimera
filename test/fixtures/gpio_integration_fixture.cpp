/********************************************************************************
 *  File Name:
 *    gpio_integration_fixture.cpp
 *
 *  Description:
 *    Implements the GPIO test fixture for the Chimera backend driver
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Chimera Includes */
#include <Chimera/gpio.hpp>

/* Testing Includes */
#include <Chimera/test/fixtures/gpio_integration.hpp>

namespace Chimera
{
  namespace GPIO
  {
    void GPIOIntegrationTestFixture::SetUp()
    {
      gpio = new CHIMERA_INHERITED_GPIO();
    }

    void GPIOIntegrationTestFixture::TearDown()
    {
      delete gpio;
    }

    void GPIOIntegrationTestFixture::defaultInit()
    {
      /*------------------------------------------------
      It's expected that a device should have at least 1
      pin on the first GPIO register.
      ------------------------------------------------*/
      gpio->init( Port::PORTA, 0 );
    }
  }  // namespace GPIO
}  // namespace Chimera
