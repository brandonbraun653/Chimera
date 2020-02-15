/********************************************************************************
 *  File Name:
 *    usart.hpp
 * 
 *  Description:
 *    Implements an interface to create a Chimera USART peripheral
 * 
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_USART_HPP
#define CHIMERA_USART_HPP

/* STL Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/src/serial/serial_intf.hpp>

namespace Chimera::USART
{
  using USART_sPtr = std::shared_ptr<::Chimera::Serial::HWInterface>;
  using USART_uPtr = std::unique_ptr<::Chimera::Serial::HWInterface>;

  Chimera::Status_t initialize();

  USART_sPtr create_shared_ptr();

  USART_uPtr create_unique_ptr();

}  // namespace Chimera::USART

#endif  /* !CHIMERA_USART_HPP */
