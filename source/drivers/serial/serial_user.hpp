/********************************************************************************
 *   File Name:
 *    serial_user.hpp
 *
 *   Description:
 *    User interface to the serial drivers
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
#include <Chimera/source/drivers/serial/serial_intf.hpp>

namespace Chimera::Serial
{
  Chimera::Status_t initialize();
  Chimera::Status_t reset();
  Driver_rPtr getDriver( const Channel channel );
}  // namespace Chimera::Serial

#endif  /* !CHIMERA_SERIAL_USER_HPP */
