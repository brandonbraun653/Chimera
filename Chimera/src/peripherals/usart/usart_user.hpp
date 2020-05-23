/********************************************************************************
 *  File Name:
 *    usart_user.hpp
 *
 *  Description:
 *    Implements an interface to create a Chimera USART peripheral
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_USART_USER_HPP
#define CHIMERA_USART_USER_HPP

/* STL Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/src/peripherals/usart/usart_intf.hpp>

namespace Chimera::USART
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
   *  Checks if the given channel is a USART channel or not
   *
   *  @param[in]  channel     The serial channel to be checked
   *  @return bool
   */
  bool isChannelUSART( const Chimera::Serial::Channel channel );

  /**
   *  Factory function that creates a shared_ptr instance of the backend
   *  driver, as long as it conforms to the expected interface.
   *
   *  @return USART_sPtr
   */
  USART_sPtr create_shared_ptr();

  /**
   *  Factory function that creates a unique_ptr instance of the backend
   *  driver, as long as it conforms to the expected interface.
   *
   *  @return USART_uPtr
   */
  USART_uPtr create_unique_ptr();

}  // namespace Chimera::USART

#endif /* !CHIMERA_USART_USER_HPP */
