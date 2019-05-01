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


using namespace Chimera::GPIO;
using namespace Chimera::Hardware;

static constexpr uint8_t invalidChannel           = 255u;
static constexpr uint8_t validChannel             = 1u;
static constexpr uint8_t possiblePeriphsUnderTest = 2u;
static constexpr Chimera::Serial::IOPins validPins{ { Pull::NO_PULL, Port::PORTA, Drive::INPUT, State::LOW, 0u, 0u },
                                                    { Pull::NO_PULL, Port::PORTA, Drive::INPUT, State::LOW, 0u, 0u } };

namespace Chimera
{
  namespace Serial
  {
    /*------------------------------------------------
    Chimera::Serial::Interface::assignHW()
    ------------------------------------------------*/
    TEST_F( SerialIntegrationTestFixture, assignHW_badChannel )
    {
      auto expected = Chimera::CommonStatusCodes::FAIL;

      Chimera::Serial::IOPins pins;
      pins.rx.pin  = 0;
      pins.rx.port = GPIO::Port::PORTA;
      pins.tx.pin  = 0;
      pins.tx.port = GPIO::Port::PORTA;

      EXPECT_EQ( expected, serial->assignHW( invalidChannel, pins ) );
    }

    TEST_F( SerialIntegrationTestFixture, assignHW_badPins )
    {
      auto expected = Chimera::CommonStatusCodes::INVAL_FUNC_PARAM;

      Chimera::Serial::IOPins pins;
      pins.rx.pin  = 0;
      pins.rx.port = GPIO::Port::UNKNOWN_PORT;
      pins.tx.pin  = 0;
      pins.tx.port = GPIO::Port::UNKNOWN_PORT;

      EXPECT_EQ( expected, serial->assignHW( validChannel, pins ) );
    }

    TEST_F( SerialIntegrationTestFixture, assignHW_pass )
    {
      auto expected = Chimera::CommonStatusCodes::OK;

      Chimera::Serial::IOPins pins;
      pins.rx.pin  = 0;
      pins.rx.port = GPIO::Port::PORTA;
      pins.tx.pin  = 0;
      pins.tx.port = GPIO::Port::PORTA;

      EXPECT_EQ( expected, serial->assignHW( getValidUARTChannel(), pins ) );
      EXPECT_EQ( expected, serial->assignHW( getValidUSARTChannel(), pins ) );
    }

    /*------------------------------------------------
    Chimera::Serial::Interface::begin()
    ------------------------------------------------*/
    TEST_F( SerialIntegrationTestFixture, begin_notInitializedYet )
    {
      auto expected1 = Chimera::CommonStatusCodes::NOT_INITIALIZED;
      auto expected2 = Chimera::CommonStatusCodes::FAIL;

      auto result = serial->begin( SubPeripheralMode::BLOCKING, SubPeripheralMode::BLOCKING );
      EXPECT_TRUE( ( result == expected1 ) || ( result == expected2 ) );

      reset_test();
      result = serial->begin( SubPeripheralMode::INTERRUPT, SubPeripheralMode::INTERRUPT );
      EXPECT_TRUE( ( result == expected1 ) || ( result == expected2 ) );

      reset_test();
      result = serial->begin( SubPeripheralMode::DMA, SubPeripheralMode::DMA );
      EXPECT_TRUE( ( result == expected1 ) || ( result == expected2 ) );

      reset_test();
      result = serial->begin( SubPeripheralMode::UNKNOWN_MODE, SubPeripheralMode::UNKNOWN_MODE );
      EXPECT_TRUE( ( result == expected1 ) || ( result == expected2 ) );
    }

    TEST_F( SerialIntegrationTestFixture, begin_invalidModes )
    {
      uint8_t channel = invalidChannel;

      for ( uint8_t x = 0; x < possiblePeriphsUnderTest; x++ )
      {
        reset_test();

        if ( !x )
        {
          channel = getValidUARTChannel();
        }
        else
        {
          channel = getValidUSARTChannel();
        }

        serial->assignHW( channel, validPins );

        auto try1 = serial->begin( SubPeripheralMode::UNKNOWN_MODE, SubPeripheralMode::BLOCKING );
        EXPECT_TRUE( ( try1 == Chimera::CommonStatusCodes::FAIL ) || ( try1 == Chimera::CommonStatusCodes::NOT_INITIALIZED )
                     || ( try1 == Chimera::CommonStatusCodes::INVAL_FUNC_PARAM ) );

        auto try2 = serial->begin( SubPeripheralMode::BLOCKING, SubPeripheralMode::UNKNOWN_MODE );
        EXPECT_TRUE( ( try2 == Chimera::CommonStatusCodes::FAIL ) || ( try2 == Chimera::CommonStatusCodes::NOT_INITIALIZED )
                     || ( try2 == Chimera::CommonStatusCodes::INVAL_FUNC_PARAM ) );
      }
    }

    TEST_F( SerialIntegrationTestFixture, begin_InitRequirements )
    {
      constexpr auto expected_fail = Chimera::CommonStatusCodes::NOT_INITIALIZED;
      constexpr auto expected_pass = Chimera::CommonStatusCodes::OK;

      uint8_t channel = invalidChannel;
      boost::circular_buffer<uint8_t> circBuf( 10 );
      std::array<uint8_t, 10> linearBuf;

      for ( uint8_t x = 0; x < possiblePeriphsUnderTest; x++ )
      {
        reset_test();

        if ( !x )
        {
          channel = getValidUARTChannel();
        }
        else
        {
          channel = getValidUSARTChannel();
        }

        /*------------------------------------------------
        Validate that assigning hardware alone won't pass begin()
        ------------------------------------------------*/
        ASSERT_EQ( expected_pass, serial->assignHW( channel, validPins ) );

        EXPECT_EQ( expected_fail, serial->begin( SubPeripheralMode::BLOCKING, SubPeripheralMode::BLOCKING ) );
        EXPECT_EQ( expected_fail, serial->begin( SubPeripheralMode::INTERRUPT, SubPeripheralMode::BLOCKING ) );
        EXPECT_EQ( expected_fail, serial->begin( SubPeripheralMode::DMA, SubPeripheralMode::BLOCKING ) );
        EXPECT_EQ( expected_fail, serial->begin( SubPeripheralMode::BLOCKING, SubPeripheralMode::INTERRUPT ) );
        EXPECT_EQ( expected_fail, serial->begin( SubPeripheralMode::BLOCKING, SubPeripheralMode::DMA ) );

        /*------------------------------------------------
        Assigning hardware + configuring the peripheral will
        pass blocking mode, but not interrupt or DMA.
        ------------------------------------------------*/
        reset_test();

        ASSERT_EQ( expected_pass, serial->assignHW( channel, validPins ) );
        ASSERT_EQ( expected_pass, serial->configure( 9600, CharWid::CW_8BIT, Parity::PAR_NONE, StopBits::SBITS_ONE,
                                                     FlowControl::FCTRL_NONE ) );

        EXPECT_EQ( expected_pass, serial->begin( SubPeripheralMode::BLOCKING, SubPeripheralMode::BLOCKING ) );
        EXPECT_EQ( expected_fail, serial->begin( SubPeripheralMode::INTERRUPT, SubPeripheralMode::BLOCKING ) );
        EXPECT_EQ( expected_fail, serial->begin( SubPeripheralMode::DMA, SubPeripheralMode::BLOCKING ) );
        EXPECT_EQ( expected_fail, serial->begin( SubPeripheralMode::BLOCKING, SubPeripheralMode::INTERRUPT ) );
        EXPECT_EQ( expected_fail, serial->begin( SubPeripheralMode::BLOCKING, SubPeripheralMode::DMA ) );

        /*------------------------------------------------
        Performing the full initialization for TX passes, but not RX
        ------------------------------------------------*/
        reset_test();

        ASSERT_EQ( expected_pass, serial->assignHW( channel, validPins ) );
        ASSERT_EQ( expected_pass, serial->configure( 9600, CharWid::CW_8BIT, Parity::PAR_NONE, StopBits::SBITS_ONE,
                                                     FlowControl::FCTRL_NONE ) );
        ASSERT_EQ( expected_pass,
                   serial->enableBuffering( Hardware::SubPeripheral::TX, &circBuf, linearBuf.data(), linearBuf.size() ) );

        EXPECT_EQ( expected_pass, serial->begin( SubPeripheralMode::BLOCKING, SubPeripheralMode::BLOCKING ) );
        EXPECT_EQ( expected_pass, serial->begin( SubPeripheralMode::INTERRUPT, SubPeripheralMode::BLOCKING ) );
        EXPECT_EQ( expected_pass, serial->begin( SubPeripheralMode::DMA, SubPeripheralMode::BLOCKING ) );
        EXPECT_EQ( expected_fail, serial->begin( SubPeripheralMode::BLOCKING, SubPeripheralMode::INTERRUPT ) );
        EXPECT_EQ( expected_fail, serial->begin( SubPeripheralMode::BLOCKING, SubPeripheralMode::DMA ) );

        /*------------------------------------------------
        Performing the full initialization for RX passes, but not TX
        ------------------------------------------------*/
        reset_test();

        ASSERT_EQ( expected_pass, serial->assignHW( channel, validPins ) );
        ASSERT_EQ( expected_pass, serial->configure( 9600, CharWid::CW_8BIT, Parity::PAR_NONE, StopBits::SBITS_ONE,
                                                     FlowControl::FCTRL_NONE ) );
        ASSERT_EQ( expected_pass,
                   serial->enableBuffering( Hardware::SubPeripheral::RX, &circBuf, linearBuf.data(), linearBuf.size() ) );

        EXPECT_EQ( expected_pass, serial->begin( SubPeripheralMode::BLOCKING, SubPeripheralMode::BLOCKING ) );
        EXPECT_EQ( expected_fail, serial->begin( SubPeripheralMode::INTERRUPT, SubPeripheralMode::BLOCKING ) );
        EXPECT_EQ( expected_fail, serial->begin( SubPeripheralMode::DMA, SubPeripheralMode::BLOCKING ) );
        EXPECT_EQ( expected_pass, serial->begin( SubPeripheralMode::BLOCKING, SubPeripheralMode::INTERRUPT ) );
        EXPECT_EQ( expected_pass, serial->begin( SubPeripheralMode::BLOCKING, SubPeripheralMode::DMA ) );

        /*------------------------------------------------
        Full initialization passes
        ------------------------------------------------*/
        reset_test();

        ASSERT_EQ( expected_pass, serial->assignHW( channel, validPins ) );
        ASSERT_EQ( expected_pass, serial->configure( 9600, CharWid::CW_8BIT, Parity::PAR_NONE, StopBits::SBITS_ONE,
                                                     FlowControl::FCTRL_NONE ) );
        ASSERT_EQ( expected_pass,
                   serial->enableBuffering( Hardware::SubPeripheral::TX, &circBuf, linearBuf.data(), linearBuf.size() ) );
        ASSERT_EQ( expected_pass,
                   serial->enableBuffering( Hardware::SubPeripheral::RX, &circBuf, linearBuf.data(), linearBuf.size() ) );

        EXPECT_EQ( expected_pass, serial->begin( SubPeripheralMode::BLOCKING, SubPeripheralMode::BLOCKING ) );
        EXPECT_EQ( expected_pass, serial->begin( SubPeripheralMode::INTERRUPT, SubPeripheralMode::BLOCKING ) );
        EXPECT_EQ( expected_pass, serial->begin( SubPeripheralMode::DMA, SubPeripheralMode::BLOCKING ) );
        EXPECT_EQ( expected_pass, serial->begin( SubPeripheralMode::BLOCKING, SubPeripheralMode::INTERRUPT ) );
        EXPECT_EQ( expected_pass, serial->begin( SubPeripheralMode::BLOCKING, SubPeripheralMode::DMA ) );
      }
    }

    /*------------------------------------------------
    Chimera::Serial::Interface::end()
    ------------------------------------------------*/
    TEST_F( SerialIntegrationTestFixture, end_Pass )
    {
      /*------------------------------------------------
      I'm not really sure how to guarantee a failed scenario for this
      function, so I'm leaving that up to the backend driver.
      ------------------------------------------------*/
      EXPECT_EQ( Chimera::CommonStatusCodes::OK, serial->end() );
    }

    /*------------------------------------------------
    Chimera::Serial::Interface::configure()
    ------------------------------------------------*/
    TEST_F( SerialIntegrationTestFixture, configure_PreInit )
    {
      EXPECT_TRUE(false);
    }

    TEST_F( SerialIntegrationTestFixture, configure_BadParams )
    {
      EXPECT_TRUE(false);
    }

    TEST_F( SerialIntegrationTestFixture, configure_Pass )
    {
      EXPECT_TRUE(false);
    }

    /*------------------------------------------------
    Chimera::Serial::Interface::setBaud()
    ------------------------------------------------*/
    TEST_F( SerialIntegrationTestFixture, setBaud_PreInit )
    {
      EXPECT_TRUE(false);
    }

    TEST_F( SerialIntegrationTestFixture, setBaud_Pass )
    {
      EXPECT_TRUE(false);
    }

    /*------------------------------------------------
    Chimera::Serial::Interface::setMode()
    ------------------------------------------------*/
    TEST_F( SerialIntegrationTestFixture, setMode_PreInit )
    {
      EXPECT_TRUE(false);
    }

    TEST_F( SerialIntegrationTestFixture, setMode_BadParams )
    {
      EXPECT_TRUE(false);
    }

    TEST_F( SerialIntegrationTestFixture, setMode_Pass )
    {
      EXPECT_TRUE(false);
    }

    /*------------------------------------------------
    Chimera::Serial::Interface::write()
    ------------------------------------------------*/
    TEST_F( SerialIntegrationTestFixture, write_PreInit )
    {
      EXPECT_TRUE(false);
    }

    TEST_F( SerialIntegrationTestFixture, write_BadParams )
    {
      EXPECT_TRUE(false);
    }

    TEST_F( SerialIntegrationTestFixture, write_Pass )
    {
      EXPECT_TRUE(false);
    }

    /*------------------------------------------------
    Chimera::Serial::Interface::read()
    ------------------------------------------------*/
    TEST_F( SerialIntegrationTestFixture, read_PreInit )
    {
      EXPECT_TRUE(false);
    }

    TEST_F( SerialIntegrationTestFixture, read_BadParams )
    {
      EXPECT_TRUE(false);
    }

    TEST_F( SerialIntegrationTestFixture, read_Pass )
    {
      EXPECT_TRUE(false);
    }

    /*------------------------------------------------
    Chimera::Serial::Interface::flush()
    ------------------------------------------------*/
    TEST_F( SerialIntegrationTestFixture, flush_PreInit )
    {
      EXPECT_TRUE(false);
    }

    TEST_F( SerialIntegrationTestFixture, flush_BadParams )
    {
      EXPECT_TRUE(false);
    }

    TEST_F( SerialIntegrationTestFixture, flush_Pass )
    {
      EXPECT_TRUE(false);
    }

    /*------------------------------------------------
    Chimera::Serial::Interface::readAsync()
    ------------------------------------------------*/
    TEST_F( SerialIntegrationTestFixture, readAsync_PreInit )
    {
      EXPECT_TRUE(false);
    }

    TEST_F( SerialIntegrationTestFixture, readAsync_BadParams )
    {
      EXPECT_TRUE(false);
    }

    TEST_F( SerialIntegrationTestFixture, readAsync_Pass )
    {
      EXPECT_TRUE(false);
    }

    /*------------------------------------------------
    Chimera::Serial::Interface::enableBuffering()
    ------------------------------------------------*/
    TEST_F( SerialIntegrationTestFixture, enableBuffering_PreInit )
    {
      EXPECT_TRUE(false);
    }

    TEST_F( SerialIntegrationTestFixture, enableBuffering_BadParams )
    {
      EXPECT_TRUE(false);
    }

    TEST_F( SerialIntegrationTestFixture, enableBuffering_Pass )
    {
      EXPECT_TRUE(false);
    }

    /*------------------------------------------------
    Chimera::Serial::Interface::disableBuffering()
    ------------------------------------------------*/
    TEST_F( SerialIntegrationTestFixture, disableBuffering_PreInit )
    {
      EXPECT_TRUE(false);
    }

    TEST_F( SerialIntegrationTestFixture, disableBuffering_BadParams )
    {
      EXPECT_TRUE(false);
    }

    TEST_F( SerialIntegrationTestFixture, disableBuffering_Pass )
    {
      EXPECT_TRUE(false);
    }

    /*------------------------------------------------
    Chimera::Serial::Interface::available()
    ------------------------------------------------*/
    TEST_F( SerialIntegrationTestFixture, available_PreInit )
    {
      EXPECT_TRUE(false);
    }

    TEST_F( SerialIntegrationTestFixture, available_BadParams )
    {
      EXPECT_TRUE(false);
    }

    TEST_F( SerialIntegrationTestFixture, available_Pass )
    {
      EXPECT_TRUE(false);
    }


  }  // namespace Serial
}  // namespace Chimera
