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
#include <Chimera/src/peripherals/crc/crc_intf.hpp>
#include <Chimera/src/peripherals/crc/crc_types.hpp>

namespace Chimera::HWCRC
{
  Chimera::Status_t initialize();
  Chimera::Status_t reset();
  IHWCRC_sPtr getDriver( const Channel channel );
}  // namespace Chimera

#endif /* !CHIMERA_CRC_HPP */
