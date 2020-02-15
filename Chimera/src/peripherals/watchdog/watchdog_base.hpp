/********************************************************************************
 *  File Name:
 *    watchdog_base.hpp
 *
 *  Description:
 *    Abstract base class interfaces
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_Watchdog_ABSTRACT_BASE_HPP
#define CHIMERA_Watchdog_ABSTRACT_BASE_HPP

/* STL Includes */
#include <cstdint>

/* Chimera Includes*/
#include <Chimera/thread>
#include <Chimera/src/peripherals/watchdog/watchdog_intf.hpp>

namespace Chimera::Watchdog
{
  class WatchdogUnsupported : virtual public IWatchdog,
                              public Chimera::Threading::Lockable
  {
  public:
    WatchdogUnsupported()  = default;
    ~WatchdogUnsupported() = default;

    Status_t initialize( const uint32_t timeout_mS, const uint8_t windowPercent ) final override
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    Status_t start() final override
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    Status_t stop() final override
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    Status_t kick() final override
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    Status_t pauseOnDebugHalt( const bool enable ) final override
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    size_t getTimeout() final override
    {
      return 0;
    }

    size_t maxTimeout() final override
    {
      return std::numeric_limits<size_t>::max();
    }

    size_t minTimeout() final override
    {
      return std::numeric_limits<size_t>::min();
    }
  };
}  // namespace Chimera::Watchdog

#endif /* !CHIMERA_Watchdog_ABSTRACT_BASE_HPP */