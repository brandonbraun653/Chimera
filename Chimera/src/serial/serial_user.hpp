/********************************************************************************
 *   File Name:
 *    serial_user.hpp
 *
 *   Description:
 *    Defines the interface to the UART/USART hardware
 *
 *   2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SERIAL_USER_HPP
#define CHIMERA_SERIAL_USER_HPP

/* STL Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/event>
#include <Chimera/thread>
#include <Chimera/src/serial/serial_intf.hpp>

namespace Chimera::Serial
{
  using Serial_sPtr = std::shared_ptr<ISerial>;
  using Serial_uPtr = std::unique_ptr<ISerial>;

  Chimera::Status_t initialize();

  Serial_sPtr create_shared_ptr( const Chimera::Serial::Channel channel );

  Serial_uPtr create_unique_ptr( const Chimera::Serial::Channel channel );

}  // namespace Chimera::Serial

#endif  /* !CHIMERA_SERIAL_USER_HPP */
