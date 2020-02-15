/********************************************************************************
 *   File Name:
 *    crc_base.hpp
 *
 *   Description:
 *    Abstract base class interfaces
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_CRC_ABSTRACT_BASE_HPP
#define CHIMERA_CRC_ABSTRACT_BASE_HPP

/* STL Includes */
#include <cstdint>

/* Chimera Includes*/
#include <Chimera/interface/crc_intf.hpp>

namespace Chimera::AlgCRC
{
  class HWInterfaceUnsupported : public HWInterface
    {
    public:
      HWInterfaceUnsupported()  = default;
      ~HWInterfaceUnsupported() = default;

      Chimera::Status_t init( const uint32_t polynomial, const uint8_t crcWidth ) final override
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }

      uint32_t accumulate( const uint32_t *const buffer, const uint32_t length ) final override
      {
        return 0u;
      }

      uint32_t calculate( const uint32_t *const buffer, const uint32_t length ) final override
      {
        return 0u;
      }
    };
}

#endif /* !CHIMERA_CRC_ABSTRACT_BASE_HPP */