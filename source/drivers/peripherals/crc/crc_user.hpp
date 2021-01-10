/********************************************************************************
 *   File Name:
 *    crc_user.hpp
 *
 *   Description:
 *    Implements an interface to create a Chimera CRC peripheral
 *
 *   2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_CRC_HPP
#define CHIMERA_CRC_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/source/drivers/peripherals/crc/crc_intf.hpp>
#include <Chimera/source/drivers/peripherals/crc/crc_types.hpp>

namespace Chimera::CRC
{
  Chimera::Status_t initialize();
  Chimera::Status_t reset();
  Driver_rPtr getDriver( const Channel channel );
}  // namespace Chimera

#endif /* !CHIMERA_CRC_HPP */
