/********************************************************************************
 *  File Name:
 *    test_serial_integration.cpp
 *
 *  Description:
 *    Tests the Chimera backend driver integration for Serial
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
#include <Chimera/test/fixtures/serial_integration_fixture.hpp>

namespace Chimera
{
  namespace Serial
  {
    /*------------------------------------------------
    Chimera::Serial::assignHW()
    ------------------------------------------------*/
    TEST_F( SerialIntegrationTestFixture, assignHW_badChannel )
    {
      auto expected = Chimera::CommonStatusCodes::FAIL;

      Chimera::Serial::IOPins pins;
      pins.rx.pin  = 0;
      pins.rx.port = GPIO::Port::PORTA;
      pins.tx.pin  = 0;
      pins.tx.port = GPIO::Port::PORTA;

      /* There cannot possibly be 255 serial channels on one device. That's nuts. */
      EXPECT_EQ( expected, serial->assignHW( 255, pins ) );
    }

    TEST_F( SerialIntegrationTestFixture, assignHW_badPins )
    {
      auto expected = Chimera::CommonStatusCodes::FAIL;

      Chimera::Serial::IOPins pins;
      pins.rx.pin  = 0;
      pins.rx.port = GPIO::Port::UNKNOWN_PORT;
      pins.tx.pin  = 0;
      pins.tx.port = GPIO::Port::UNKNOWN_PORT;

      EXPECT_EQ( expected, serial->assignHW( 1, pins ) );
    }

    TEST_F( SerialIntegrationTestFixture, assignHW_pass )
    {
      auto expected = Chimera::CommonStatusCodes::OK;

      Chimera::Serial::IOPins pins;
      pins.rx.pin  = 0;
      pins.rx.port = GPIO::Port::PORTA;
      pins.tx.pin  = 0;
      pins.tx.port = GPIO::Port::PORTA;

      EXPECT_EQ( expected, serial->assignHW( 1, pins ) );
    }


  }  // namespace Serial
}  // namespace Chimera
