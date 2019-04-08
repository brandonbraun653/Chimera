/********************************************************************************
 *   File Name:
 *       watchdog.hpp
 *
 *   Description:
 *       Provides the interface for generic watchdog functionality
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_WATCHDOG_HPP
#define CHIMERA_WATCHDOG_HPP

/* C++ Includes */
#include <cstdint>
#include <memory>
#include <type_traits>

/* Chimera Includes */
#include "chimeraPort.hpp"
#include <Chimera/interface.hpp>

namespace Chimera
{
  namespace Watchdog
  {
    class WatchdogClass : public CHIMERA_INHERITED_WATCHDOG
    {
    public:
      WatchdogClass()  = default;
      ~WatchdogClass() = default;
    };

    using WatchdogClass_sPtr = std::shared_ptr<WatchdogClass>;
    using WatchdogClass_uPtr = std::unique_ptr<WatchdogClass>;

    static_assert( std::is_base_of<Interface, WatchdogClass>::value, "Class implements wrong interface" );
  }  // namespace Watchdog
}  // namespace Chimera

#endif /* !CHIMERA_WATCHDOG_HPP */