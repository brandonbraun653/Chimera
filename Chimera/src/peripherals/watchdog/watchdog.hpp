/********************************************************************************
 *   File Name:
 *       watchdog.hpp
 *
 *   Description:
 *       Implements an interface to create a Chimera Watchdog peripheral
 *
 *   2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_WATCHDOG_HPP
#define CHIMERA_WATCHDOG_HPP

/* STL Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/peripherals/watchdog/watchdog_intf.hpp>

namespace Chimera::Watchdog
{
  using Watchdog_sPtr = std::shared_ptr<HWInterface>;
  using Watchdog_uPtr = std::unique_ptr<HWInterface>;

  Chimera::Status_t initialize();

  Watchdog_sPtr create_shared_ptr();

  Watchdog_uPtr create_unique_ptr();

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