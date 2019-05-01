/********************************************************************************
 *  File Name:
 *    common_fixture.hpp
 *
 *  Description:
 *    Defines common resources uses in the Chimera integration test fixtures. These
 *    must be implemented in the Backend driver somewhere.
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_COMMON_INTEGRATION_TEST_FIXTURE_HPP
#define CHIMERA_COMMON_INTEGRATION_TEST_FIXTURE_HPP

/* C++ Includes */
#include <cstdint>


namespace Chimera::IntegrationTests
{
  /**
   *	A stub function that allows the Chimera backend to provide
   *  custom setup functionality that will be run with every test
   *  fixture.
   *
   *	@return void
   */
  extern void CommonSetUp();

  /**
   *	A stub function that allows the Chimera backend to provide
   *  custom tear down functionality that will be run with every test
   *  fixture.
   *
   *	@return void
   */
  extern void CommonTearDown();

  namespace Serial
  {
    /**
     *	Hooks into the Backend test fixture to provide a valid UART channel. Some
     *	libraries may combine the USART and UART drivers into the Serial driver
     *	for interface consistency reasons.
     *
     *	@return extern uint8_t
     */
    extern uint8_t validUARTChannel();

    /**
     *	Hooks into the Backend test fixture to provide a valid USART channel. Some
     *	libraries may combine the USART and UART drivers into the Serial driver
     *	for interface consistency reasons.
     *
     *	@return extern uint8_t
     */
    extern uint8_t validUSARTChannel();
  }
  
}  // namespace Chimera::IntegrationTests


#endif /* !CHIMERA_COMMON_INTEGRATION_TEST_FIXTURE_HPP */