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

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/source/drivers/peripherals/uart/uart_intf.hpp>
#include <Chimera/source/drivers/peripherals/uart/uart_types.hpp>

namespace Chimera::UART
{
  Chimera::Status_t initialize();
  Chimera::Status_t reset();
  bool isChannelUART( const Chimera::Serial::Channel channel );
  Driver_sPtr getDriver( const Chimera::Serial::Channel channel );
}  // namespace Chimera::UART

#endif  /* !CHIMERA_UART_HPP */
