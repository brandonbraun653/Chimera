/********************************************************************************
 *  File Name:
 *    spi_integration_fixture.cpp
 *
 *  Description:
 *    Implements the SPI test fixture for the Chimera backend driver
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Chimera Includes */
#include <Chimera/spi.hpp>

/* Testing Includes */
#include <Chimera/test/fixtures/spi_integration.hpp>

namespace Chimera
{
  namespace SPI
  {
    void SPIIntegrationTestFixture::SetUp()
    {
      spi = new CHIMERA_INHERITED_SPI();
    }

    void SPIIntegrationTestFixture::TearDown()
    {
      delete spi;
    }

    void SPIIntegrationTestFixture::defaultInit()
    {
    
    }

  }  // namespace SPI
}  // namespace Chimera