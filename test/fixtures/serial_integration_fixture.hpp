/********************************************************************************
 *  File Name:
 *    serial_integration_fixture.hpp
 *
 *  Description:
 *    Implements the Serial test fixture for the Chimera backend driver
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SERIAL_INTEGRATION_TEST_FIXTURE_HPP
#define CHIMERA_SERIAL_INTEGRATION_TEST_FIXTURE_HPP

/* Testing Includes */
#include <gtest/gtest.h>

/* Chimera Includes */
#include "chimeraPort.hpp"

namespace Chimera::Serial
{
  class SerialIntegrationTestFixture : public ::testing::Test
  {
  protected:
    virtual void SetUp() override;

    virtual void TearDown() override;

    /**
     *	Passes the Serial initialization sequence
     *
     *	@return void
     */
    void defaultInit();

    /**
     *	Resets the testing environment manually
     *	
     *	@return void
     */
     void reset_test();

    /**
     *	Hooks into the Backend test fixture to provide a valid UART channel. Some 
     *	libraries may combine the USART and UART drivers into the Serial driver
     *	for interface consistency reasons.
     *	
     *	@return uint8_t
     */
     uint8_t getValidUARTChannel();

    /**
     *	Hooks into the Backend test fixture to provide a valid USART channel. Some 
     *	libraries may combine the USART and UART drivers into the Serial driver
     *	for interface consistency reasons.
     *	
     *	@return uint8_t
     */
     uint8_t getValidUSARTChannel();

    CHIMERA_INHERITED_SERIAL *serial = nullptr;
  };
}  // namespace Chimera::Serial

#endif  // !CHIMERA_SERIAL_INTEGRATION_TEST_FIXTURE_HPP
