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
#include <Chimera/src/peripherals/uart/uart_intf.hpp>

namespace Chimera::UART
{
  Chimera::Status_t initialize();

  Chimera::Status_t reset();

  UART_sPtr create_shared_ptr();

  UART_uPtr create_unique_ptr();

}  // namespace Chimera::UART

#endif  /* !CHIMERA_UART_HPP */
