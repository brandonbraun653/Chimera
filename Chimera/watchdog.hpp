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
#include <Chimera/config.hpp>
#include <Chimera/interface.hpp>

namespace Chimera
{
  namespace Watchdog
  {
#ifndef CHIMERA_INHERITED_WATCHDOG
    typedef Interface CHIMERA_INHERITED_WATCHDOG;
#endif

    class WatchdogClass : public CHIMERA_INHERITED_WATCHDOG
    {
    public:
      WatchdogClass()  = default;
      ~WatchdogClass() = default;

    private:
    };

    typedef std::shared_ptr<Chimera::Watchdog::WatchdogClass> WatchdogClass_sPtr;
    typedef std::unique_ptr<Chimera::Watchdog::WatchdogClass> WatchdogClass_uPtr;

    static_assert( std::is_base_of<Interface, WatchdogClass>::value,
                   "CHIMERA: Base class does not implement the correct interface" );
  }  // namespace Watchdog
}  // namespace Chimera

#endif /* !CHIMERA_WATCHDOG_HPP */