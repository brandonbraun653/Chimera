/********************************************************************************
 *   File Name:
 *    watchdog_types.hpp
 *
 *   Description:
 *    Chimera Watchdog types
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_WATCHDOG_TYPES_HPP
#define CHIMERA_WATCHDOG_TYPES_HPP

/* STL Includes */
#include <cstdint>
#include <memory>

/* Chimera Includes */
#include <Chimera/types/common_types.hpp>

namespace Chimera
{
   namespace Watchdog
  {
    class WatchdogClass;
    using WatchdogClass_sPtr = std::shared_ptr<WatchdogClass>;
    using WatchdogClass_uPtr = std::unique_ptr<WatchdogClass>;
    
  }
}  // namespace Chimera

#endif /* !CHIMERA_WATCHDOG_TYPES_HPP */