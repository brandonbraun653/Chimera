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
#include <Chimera/test/fixtures/common_fixture.hpp>
#include <Chimera/test/fixtures/gpio_integration_fixture.hpp>

namespace Chimera
{
  namespace GPIO
  {
    void GPIOIntegrationTestFixture::SetUp()
    {
      gpio = new CHIMERA_INHERITED_GPIO();

      Chimera::IntegrationTests::CommonSetUp();
    }

    void GPIOIntegrationTestFixture::TearDown()
    {
      delete gpio;

      Chimera::IntegrationTests::CommonTearDown();
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
