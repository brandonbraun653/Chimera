/********************************************************************************
 *   File Name:
 *    watchdog_base.hpp
 *
 *   Description:
 *    Abstract base class interfaces
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_Watchdog_ABSTRACT_BASE_HPP
#define CHIMERA_Watchdog_ABSTRACT_BASE_HPP

/* STL Includes */
#include <cstdint>

/* Chimera Includes*/
#include <Chimera/interface/watchdog_intf.hpp>

namespace Chimera::Watchdog
{
  class WatchdogUnsupported : public HWInterface
  {
  public:
    WatchdogUnsupported()  = default;
    ~WatchdogUnsupported() = default;

    Status_t initialize( const uint32_t timeout_mS, const uint8_t windowPercent )
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    Status_t start()
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    Status_t stop()
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    Status_t kick()
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    Status_t pauseOnDebugHalt( const bool enable )
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    size_t getTimeout()
    {
      return 0;
    }

    size_t maxTimeout()
    {
      return std::numeric_limits<size_t>::max();
    }

    size_t minTimeout()
    {
      return std::numeric_limits<size_t>::min();
    }
  };
}  // namespace Chimera::Watchdog

#endif /* !CHIMERA_Watchdog_ABSTRACT_BASE_HPP */