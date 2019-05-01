/********************************************************************************
 *  File Name:
 *    serial_integration_fixture.cpp
 *
 *  Description:
 *    Implements the Serial test fixture for the Chimera backend driver
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Chimera Includes */
#include <Chimera/gpio.hpp>

/* Testing Includes */
#include <Chimera/test/fixtures/common_fixture.hpp>
#include <Chimera/test/fixtures/serial_integration_fixture.hpp>

namespace Chimera::Serial
{
    void SerialIntegrationTestFixture::SetUp()
    {
      serial = new CHIMERA_INHERITED_SERIAL();

      Chimera::IntegrationTests::CommonSetUp();
    }

    void SerialIntegrationTestFixture::TearDown()
    {
      delete serial;

      Chimera::IntegrationTests::CommonTearDown();
    }

    void SerialIntegrationTestFixture::defaultInit()
    {
    }

    void SerialIntegrationTestFixture::reset_test()
    {
      TearDown();
      SetUp();
    }

    uint8_t SerialIntegrationTestFixture::getValidUARTChannel()
    {
      return Chimera::IntegrationTests::Serial::validUARTChannel();
    }

    uint8_t SerialIntegrationTestFixture::getValidUSARTChannel()
    {
      return Chimera::IntegrationTests::Serial::validUSARTChannel();
    }
}  // namespace Chimera