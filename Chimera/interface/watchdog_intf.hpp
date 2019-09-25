/********************************************************************************
 *   File Name:
 *    watchdog_intf.hpp
 *
 *   Description:
 *    Models the Chimera Watchdog interface
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_WATCHDOG_INTERFACE_HPP
#define CHIMERA_WATCHDOG_INTERFACE_HPP

/* C++ Includes */
#include <cstdint>
#include <cstdlib>
#include <limits>

/* Chimera Includes */
#include <Chimera/types/common_types.hpp>

namespace Chimera::Watchdog
{
  class Interface
  {
  public:
    virtual ~Interface() = default;

    /**
     *  Initializes the low level hardware needed to configure the watchdog
     *  peripheral. This does not start the timer.
     *
     *  @note   Guarantees a minimum resolution of +/- 500uS around the specified timeout
     *  
     *  @param[in] timeout_mS        How many milliseconds can elapse before watchdog expires
     *  @param[in] windowPercent     Percentage away from timeout expiring before dog can be kicked
     *  @return Status::OK if the initialization was a success, Status::FAIL if not
     */
    virtual Status_t initialize( const uint32_t timeout_mS, const uint8_t windowPercent )
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    /**
     *   Starts the watchdog timer. If successful, Interface::kick() must
     *   be called at regular intervals to prevent the watchdog from firing.
     *
     *   @return Chimera::Status_t
     */
    virtual Status_t start()
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    /**
     *   Stops the watchdog timer.
     *
     *   @return Chimera::Status_t
     */
    virtual Status_t stop()
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    /**
     *   Kicks the watchdog timer, starting a new countdown cycle.
     *
     *   @return Chimera::Status_t
     */
    virtual Status_t kick()
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    /**
     *   Configures the watchdog to stop on connection to a debugger
     *
     *   @param[in]  enable      If true, allows the watchdog to stop. Otherwise, it continues running
     *   @return Chimera::Status_t
     */
    virtual Status_t pauseOnDebugHalt( const bool enable )
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    /**
     *   Gets the actual timeout value achieved by the hardware
     *
     *   @param[out] timeout     Timeout value in milliseconds
     *   @return Chimera::Status_t
     */
    virtual size_t getTimeout()
    {
      return 0;
    }

    /**
     *  Returns the max supported timeout by the hardware in milliseconds
     *
     *  @return size_t
     */
    virtual size_t maxTimeout()
    {
      return std::numeric_limits<size_t>::max();
    }

    /**
     *  Returns the min supported timeout by the hardware in milliseconds
     *
     *  @return size_t
     */
    virtual size_t minTimeout()
    {
      return std::numeric_limits<size_t>::min();
    }
  };
}  // namespace Chimera::Watchdog

#endif /* !CHIMERA_WATCHDOG_INTERFACE_HPP */