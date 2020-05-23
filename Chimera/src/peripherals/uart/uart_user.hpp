/********************************************************************************
 *  File Name:
 *    uart_user.hpp
 *
 *  Description:
 *    Implements an interface to create a Chimera UART peripheral
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_UART_HPP
#define CHIMERA_UART_HPP

/* STL Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/src/peripherals/uart/uart_intf.hpp>

namespace Chimera::UART
{
  /**
   *  Initializes the backend driver's memory. Should really only call once for initial set up.
   *
   *  @return Chimera::Status_t
   */
  Chimera::Status_t initialize();

  /**
   *  Resets the backend driver hardware to default configuration
   *  settings, but does not wipe out any memory.
   *
   *  @return Chimera::Status_t
   */
  Chimera::Status_t reset();

  /**
   *  Checks if the given channel is a UART channel or not
   *
   *  @param[in]  channel     The serial channel to be checked
   *  @return bool
   */
  bool isChannelUART( const Chimera::Serial::Channel channel );

  /**
   *  Factory function that creates a shared_ptr instance of the backend
   *  driver, as long as it conforms to the expected interface.
   *
   *  @return UART_sPtr
   */
  UART_sPtr create_shared_ptr();

  /**
   *  Factory function that creates a unique_ptr instance of the backend
   *  driver, as long as it conforms to the expected interface.
   *
   *  @return UART_uPtr
   */
  UART_uPtr create_unique_ptr();

}  // namespace Chimera::UART

#endif  /* !CHIMERA_UART_HPP */
