/********************************************************************************
 *  File Name:
 *    test_gpio_integration.cpp
 *
 *  Description:
 *    Tests the Chimera backend driver integration for GPIO
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* STL Includes */
#include <limits>

/* Testing Includes */
#include <gtest/gtest.h>
#include <gmock/gmock.h>

/* Chimera Includes */
#include "chimeraPort.hpp"
#include <Chimera/test/fixtures/gpio_integration_fixture.hpp>

namespace Chimera
{
  namespace GPIO
  {
    /*------------------------------------------------
    Chimera::GPIO::init()
    ------------------------------------------------*/
    TEST_F( GPIOIntegrationTestFixture, init_badPort )
    {
      auto expected = Chimera::CommonStatusCodes::INVAL_FUNC_PARAM;
      EXPECT_EQ( expected, gpio->init( Port::NUM_PORTS, 0 ) );
    }

    TEST_F( GPIOIntegrationTestFixture, init_badPin )
    {
      auto expected = Chimera::CommonStatusCodes::INVAL_FUNC_PARAM;
      EXPECT_EQ( expected, gpio->init( Port::PORTA, std::numeric_limits<uint8_t>::max() ) );
    }

    TEST_F( GPIOIntegrationTestFixture, init_ok )
    {
      /*------------------------------------------------
      It's expected that a device should have at least 1
      pin on the first GPIO register. Otherwise, GPIO
      probably shouldn't be supported...
      ------------------------------------------------*/
      auto expected = Chimera::CommonStatusCodes::OK;
      EXPECT_EQ( expected, gpio->init( Port::PORTA, 0 ) );
    }

    /*------------------------------------------------
    Chimera::GPIO::setMode()
    ------------------------------------------------*/
    TEST_F( GPIOIntegrationTestFixture, setMode_badDrive )
    {
      defaultInit();
      auto expected = Chimera::CommonStatusCodes::INVAL_FUNC_PARAM;
      EXPECT_EQ( expected, gpio->setMode( Drive::UNKNOWN_DRIVE, true ) );
      EXPECT_EQ( expected, gpio->setMode( Drive::UNKNOWN_DRIVE, false ) );
    }

    TEST_F( GPIOIntegrationTestFixture, setMode_preInit )
    {
      auto expected = Chimera::CommonStatusCodes::NOT_INITIALIZED;
      EXPECT_EQ( expected, gpio->setMode( Drive::INPUT, true ) );
    }

    /*------------------------------------------------
    Chimera::GPIO::setState()
    ------------------------------------------------*/
    TEST_F( GPIOIntegrationTestFixture, setState_allVals )
    {
      defaultInit();
      auto expected = Chimera::CommonStatusCodes::OK;
      EXPECT_EQ( expected, gpio->setState( State::HI ) );
      EXPECT_EQ( expected, gpio->setState( State::HIGH ) );
      EXPECT_EQ( expected, gpio->setState( State::LO ) );
      EXPECT_EQ( expected, gpio->setState( State::LOW ) );
      EXPECT_EQ( expected, gpio->setState( State::ON ) );
      EXPECT_EQ( expected, gpio->setState( State::OFF ) );
    }

    TEST_F( GPIOIntegrationTestFixture, setState_preInit )
    {
      auto expected = Chimera::CommonStatusCodes::NOT_INITIALIZED;
      EXPECT_EQ( expected, gpio->setState( State::HI ) );
    }

    /*------------------------------------------------
    Chimera::GPIO::getState()
    ------------------------------------------------*/
    TEST_F( GPIOIntegrationTestFixture, getState_preInit )
    {
      Chimera::GPIO::State state;
      auto expected = Chimera::CommonStatusCodes::NOT_INITIALIZED;
      EXPECT_EQ( expected, gpio->getState( state ) );
    }

    TEST_F( GPIOIntegrationTestFixture, getState_returnAnything )
    {
      defaultInit();
      Chimera::GPIO::State state;
      auto expected = Chimera::CommonStatusCodes::OK;
      EXPECT_EQ( expected, gpio->getState( state ) );

      /* clang-format off */
      EXPECT_EQ( true, ( ( state == State::HI   )
                      || ( state == State::HIGH )
                      || ( state == State::ON   ) 
                      || ( state == State::LOW  )
                      || ( state == State::LO   )
                      || ( state == State::OFF  ) ) );
      /* clang-format on */
    }

    /*------------------------------------------------
    Chimera::GPIO::toggle()
    ------------------------------------------------*/
    TEST_F( GPIOIntegrationTestFixture, toggle_preInit )
    {
      auto expected = Chimera::CommonStatusCodes::NOT_INITIALIZED;
      EXPECT_EQ( expected, gpio->toggle() );
    }

    TEST_F( GPIOIntegrationTestFixture, toggle_normal )
    {
      defaultInit();
      auto expected = Chimera::CommonStatusCodes::OK;
      EXPECT_EQ( expected, gpio->toggle() );
    }

  }  // namespace GPIO
}  // namespace Chimera
