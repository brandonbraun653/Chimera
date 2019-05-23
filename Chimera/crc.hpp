/********************************************************************************
 *   File Name:
 *    crc.hpp
 *
 *   Description:
 *    Defines the interface to the CRC hardware
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_CRC_HPP
#define CHIMERA_CRC_HPP

/* C++ Includes */
#include <cstdint>
#include <memory>

/* Chimera Includes */
#include <Chimera/interface/macro.hpp>
#include <Chimera/interface/crc_intf.hpp>
#include "chimeraPort.hpp"

namespace Chimera
{
  namespace AlgCRC
  {
#if !defined( CHIMERA_INHERITED_HW_CRC )
    using CHIMERA_INHERITED_HW_CRC = HWInterfaceUnsupported;
#endif
    
    class HW : public CHIMERA_INHERITED_HW_CRC
    {
    public:
      HW()  = default;
      ~HW() = default;
    };

    static_assert( std::is_base_of<Chimera::AlgCRC::Interface, HW>::value, "Class implements wrong interface" );
    
    
    class SW : public Chimera::AlgCRC::Interface
    {
    public:
      SW();
      ~SW() = default;

      Chimera::Status_t init( const uint32_t polynomial, const uint8_t crcWidth ) final override;

      uint32_t accumulate( const uint32_t *const buffer, const uint32_t length ) final override;

      uint32_t calculate( const uint32_t *const buffer, const uint32_t length ) final override;

      uint32_t getPolynomial() final override;

    private:
      uint8_t bits;
      uint32_t poly;
      uint32_t previous;
    };
    
    static_assert( std::is_base_of<Chimera::AlgCRC::Interface, SW>::value, "Base class implements the wrong interface" );

  }  // namespace AlgCRC
}  // namespace Chimera

#endif /* !CHIMERA_CRC_HPP */