/********************************************************************************
 *  File Name:
 *    test_spi_integration.cpp
 *
 *  Description:
 *    Tests the Chimera backend driver integration for GPIO
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */
#include <limits>

/* Testing Includes */
#include <gtest/gtest.h>
#include <gmock/gmock.h>

/* Chimera Includes */
#include "chimeraPort.hpp"
#include <Chimera/test/fixtures/spi_integration_fixture.hpp>

namespace Chimera
{
  namespace SPI
  {
    /*------------------------------------------------
    Chimera::SPI::init()
    ------------------------------------------------*/
    TEST_F( SPIIntegrationTestFixture, init_bad_struct )
    {
      Chimera::SPI::Setup setup;
    }
  }  // namespace SPI
}  // namespace Chimera