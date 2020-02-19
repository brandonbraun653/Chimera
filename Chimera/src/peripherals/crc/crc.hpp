/********************************************************************************
 *   File Name:
 *    crc.hpp
 *
 *   Description:
 *    Implements an interface to create a Chimera CRC peripheral
 *
 *   2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_CRC_HPP
#define CHIMERA_CRC_HPP

/* STL Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/src/peripherals/crc/crc_intf.hpp>

namespace Chimera::HWCRC
{
  Chimera::Status_t initialize();

  Chimera::Status_t reset();

  HWCRC_sPtr create_shared_ptr();

  HWCRC_uPtr create_unique_ptr();

}  // namespace Chimera

#endif /* !CHIMERA_CRC_HPP */