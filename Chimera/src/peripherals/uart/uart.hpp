/********************************************************************************
 *  File Name:
 *    uart.hpp
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
#include <Chimera/src/serial/serial_intf.hpp>

namespace Chimera::UART
{
  using UART_sPtr = std::shared_ptr<::Chimera::Serial::HWInterface>;
  using UART_uPtr = std::unique_ptr<::Chimera::Serial::HWInterface>;

  Chimera::Status_t initialize();

  UART_sPtr create_shared_ptr();

  UART_uPtr create_unique_ptr();

}  // namespace Chimera::UART

#endif  /* !CHIMERA_UART_HPP */
