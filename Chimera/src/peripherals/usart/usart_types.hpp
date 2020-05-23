/********************************************************************************
 *  File Name:
 *    usart_types.hpp
 *
 *	Description:
 *    Types for the Chimera USART
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_USART_TYPES_HPP
#define CHIMERA_USART_TYPES_HPP

/* STL Includes */
#include <cstdint>
#include <memory>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/src/serial/serial_types.hpp>

namespace Chimera::USART
{
  class IUSART;

  using USART_sPtr = std::shared_ptr<IUSART>;
  using USART_uPtr = std::unique_ptr<IUSART>;

  namespace Backend
  {
    struct DriverConfig
    {
      bool isSupported;
      Chimera::Status_t ( *initialize )( void );
      Chimera::Status_t ( *reset )( void );
      bool ( *isChannelUSART )( const Chimera::Serial::Channel );
      USART_sPtr ( *createShared )( void );
      USART_uPtr ( *createUnique )( void );
    };
  }  // namespace Backend
}

#endif  /* !CHIMERA_USART_TYPES_HPP */
