/********************************************************************************
 *  File Name:
 *    uart_types.hpp
 *
 *	Description:
 *    Types for the Chimera UART
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_UART_TYPES_HPP
#define CHIMERA_UART_TYPES_HPP

/* STL Includes */
#include <cstdint>
#include <memory>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/src/serial/serial_types.hpp>

namespace Chimera::UART
{
  class IUART;

  using UART_sPtr = std::shared_ptr<IUART>;
  using UART_uPtr = std::unique_ptr<IUART>;

  namespace Backend
  {
    struct DriverConfig
    {
      bool isSupported;
      Chimera::Status_t ( *initialize )( void );
      Chimera::Status_t ( *reset )( void );
      bool ( *isChannelUART )( const Chimera::Serial::Channel );
      UART_sPtr ( *createShared )( void );
      UART_uPtr ( *createUnique )( void );
    };
  }  // namespace Backend
}  // namespace Chimera::UART

#endif /* !CHIMERA_UART_TYPES_HPP */
