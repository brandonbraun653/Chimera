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

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/source/drivers/peripherals/usart/usart_intf.hpp>
#include <Chimera/source/drivers/peripherals/usart/usart_types.hpp>

namespace Chimera::USART
{
  Chimera::Status_t initialize();
  Chimera::Status_t reset();
  bool isChannelUSART( const Chimera::Serial::Channel channel );
  Driver_sPtr getDriver( const Chimera::Serial::Channel channel );
}  // namespace Chimera::USART

#endif /* !CHIMERA_USART_USER_HPP */
