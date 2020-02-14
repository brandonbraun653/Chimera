/********************************************************************************
 *   File Name:
 *    serial.hpp
 *
 *   Description:
 *    Defines the interface to the UART/USART hardware
 *
 *   2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SERIAL_HPP
#define CHIMERA_SERIAL_HPP

/* C++ Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/serial/serial_intf.hpp>

namespace Chimera::Serial
{
  using Serial_sPtr = std::shared_ptr<HWInterface>;
  using Serial_uPtr = std::unique_ptr<HWInterface>;

  Chimera::Status_t initialize();

  Serial_sPtr create_shared_ptr();

  Serial_uPtr create_unique_ptr();

}  // namespace Chimera::Serial

#endif  /* !CHIMERA_SERIAL_HPP */
