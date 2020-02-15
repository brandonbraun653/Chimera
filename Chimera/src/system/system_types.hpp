/********************************************************************************
 *   File Name:
 *    system_types.hpp
 *
 *   Description:
 *    Chimera System types
 *
 *   2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SYSTEM_TYPES_HPP
#define CHIMERA_SYSTEM_TYPES_HPP

/* STL Includes */
#include <cstdint>
#include <memory>

/* Chimera Includes */
#include <Chimera/common>

namespace Chimera::System
{
    
    class SystemControl;
    using SystemControl_sPtr = std::shared_ptr<SystemControl>;
    using SystemControl_uPtr = std::unique_ptr<SystemControl>;

    class Identifier;
    using Identifier_sPtr = std::shared_ptr<Identifier>;
    using Identifier_uPtr = std::unique_ptr<Identifier>;

    enum class ResetType : uint8_t
    {
      BROWN_OUT,
      SOFTWARE,
      HW_WATCHDOG_TIMEOUT,
      UNKNOWN_RESET,
      MAX_RESET_TYPE
    };

    struct InterruptMask
    {
      bool interrupted;
      uint32_t mask;
    };
}  // namespace Chimera::System

#endif /* !CHIMERA_SYSTEM_TYPES_HPP */