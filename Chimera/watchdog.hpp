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
#include <type_traits>

/* Chimera Includes */
#include <Chimera/interface/macro.hpp>
#include <Chimera/interface/watchdog_intf.hpp>
#include "chimeraPort.hpp"

namespace Chimera
{
  namespace Watchdog
  {
#if !defined( CHIMERA_INHERITED_WATCHDOG )
    using CHIMERA_INHERITED_WATCHDOG = WatchdogUnsupported;
#endif
    
    class WatchdogClass : public CHIMERA_INHERITED_WATCHDOG
    {
    public:
      WatchdogClass()  = default;
      ~WatchdogClass() = default;
    };


    static_assert( std::is_base_of<Interface, WatchdogClass>::value, "Class implements wrong interface" );

#if !defined( CHIMERA_DISABLE_INHERITANCE_WARNINGS )
    STATIC_WARNING( !( std::is_base_of<WatchdogUnsupported, WatchdogClass>::value ),
                    "No power interface defined in backend driver. You can disable these warnings by defining "
                    "CHIMERA_DISABLE_INHERITANCE_WARNINGS in the preprocessor." );
#endif
    
  }  // namespace Watchdog
}  // namespace Chimera

#endif /* !CHIMERA_WATCHDOG_HPP */