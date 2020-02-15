/********************************************************************************
 *   File Name:
 *    crc_types.hpp
 *
 *   Description:
 *    Chimera CRC types
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_CRC_TYPES_HPP
#define CHIMERA_CRC_TYPES_HPP

/* STL Includes */
#include <cstdint>
#include <memory>

/* Chimera Includes */
#include <Chimera/types/common_types.hpp>

namespace Chimera
{
  namespace AlgCRC
  {
    class HW;
    using HW_sPtr = std::shared_ptr<HW>;
    using HW_uPtr = std::unique_ptr<HW>;
    
    class SW;
    using SW_sPtr = std::shared_ptr<SW>;
    using SW_uPtr = std::unique_ptr<SW>;
  }
}  // namespace Chimera

#endif /* !CHIMERA_CRC_TYPES_HPP */