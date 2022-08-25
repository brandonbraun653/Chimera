/********************************************************************************
 *   File Name:
 *    watchdog.hpp
 *
 *   Description:
 *    Implements an interface to create a Chimera Watchdog peripheral
 *
 *   2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_WATCHDOG_HPP
#define CHIMERA_WATCHDOG_HPP

/* Chimera Includes */
#include <Chimera/source/drivers/peripherals/watchdog/watchdog_intf.hpp>
#include <Chimera/source/drivers/peripherals/watchdog/watchdog_types.hpp>

namespace Chimera::Watchdog
{
  Chimera::Status_t initialize();
  Chimera::Status_t reset();
  Independent_rPtr getDriver( const IChannel channel );
  Window_rPtr getDriver( const WChannel channel );

  /**
   *  Intentionally halts the CPU so that the watchdog timer will reset the
   *  system. This function will never return and the next system event will
   *  be a hard reset.
   *
   *  @return void
   */
  void invokeTimeout();

}  // namespace Chimera::Watchdog

#endif /* !CHIMERA_WATCHDOG_HPP */
