/******************************************************************************
 *  File Name:
 *    usart_types.hpp
 *
 *  Description:
 *    Types for the Chimera USART
 *
 *  2020-2021 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_USART_TYPES_HPP
#define CHIMERA_USART_TYPES_HPP

/* STL Includes */
#include <cstdint>
#include <memory>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/source/drivers/serial/serial_types.hpp>

namespace Chimera::USART
{
  /*---------------------------------------------------------------------------
  Forward Declarations
  ---------------------------------------------------------------------------*/
  class Driver;

  /*---------------------------------------------------------------------------
  Aliases
  ---------------------------------------------------------------------------*/
  using Driver_rPtr = Driver *;

  /*---------------------------------------------------------------------------
  Structures
  ---------------------------------------------------------------------------*/
  namespace Backend
  {
    struct DriverConfig
    {
      bool isSupported;

      /**
       *  Initializes the backend driver's memory. Should really only call once for initial set up.
       *
       *  @return Chimera::Status_t
       */
      Chimera::Status_t ( *initialize )( void );

      /**
       *  Resets the backend driver hardware to default configuration
       *  settings, but does not wipe out any memory.
       *
       *  @return Chimera::Status_t
       */
      Chimera::Status_t ( *reset )( void );

      /**
       *  Checks if the given channel is a USART channel or not
       *
       *  @param[in]  channel     The serial channel to be checked
       *  @return bool
       */
      bool ( *isChannelUSART )( const Chimera::Serial::Channel );

      /**
       *  Factory function that gets a pointer to an instance of the backend
       *  driver, as long as it conforms to the expected interface.
       *
       *  @return IUSART_sPtr
       */
      Driver_rPtr ( *getDriver )( const Chimera::Serial::Channel );
    };
  }  // namespace Backend
}  // namespace Chimera::USART

#endif /* !CHIMERA_USART_TYPES_HPP */
