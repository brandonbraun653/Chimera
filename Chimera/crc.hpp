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
#include <Chimera/interface.hpp>

namespace Chimera
{
  namespace CRC
  {
    class HW : public CHIMERA_INHERITED_HW_CRC
    {
    public:
      HW() = default;
      ~HW() = default;
    };

    using HW_sPtr = std::shared_ptr<HW>;
    using HW_uPtr = std::unique_ptr<HW>;
    
    static_assert( std::is_base_of<Chimera::CRC::HWInterface, HW>::value, "Class implements wrong interface");

  }  // namespace CRC
}  // namespace Chimera

#endif /* !CHIMERA_CRC_HPP */