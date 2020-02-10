/********************************************************************************
 *  File Name:
 *    watchdog_intf.hpp
 *
 *  Description:
 *    Models the Chimera Watchdog interface
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_WATCHDOG_INTERFACE_HPP
#define CHIMERA_WATCHDOG_INTERFACE_HPP

/* STL Includes */
#include <cstdint>
#include <cstdlib>
#include <limits>

/* Chimera Includes */
#include <Chimera/common>

namespace Chimera::Watchdog
{
  class HWInterface
  {
  public:
    virtual ~HWInterface() = default;

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
    virtual Status_t initialize( const uint32_t timeout_mS, const uint8_t windowPercent ) = 0;

    /**
     *   Starts the watchdog timer. If successful, Interface::kick() must
     *   be called at regular intervals to prevent the watchdog from firing.
     *
     *   @return Chimera::Status_t
     */
    virtual Status_t start() = 0;

    /**
     *   Stops the watchdog timer.
     *
     *   @return Chimera::Status_t
     */
    virtual Status_t stop() = 0;

    /**
     *   Kicks the watchdog timer, starting a new countdown cycle.
     *
     *   @return Chimera::Status_t
     */
    virtual Status_t kick() = 0;

    /**
     *   Configures the watchdog to stop on connection to a debugger
     *
     *   @param[in]  enable      If true, allows the watchdog to stop. Otherwise, it continues running
     *   @return Chimera::Status_t
     */
    virtual Status_t pauseOnDebugHalt( const bool enable ) = 0;

    /**
     *   Gets the actual timeout value achieved by the hardware
     *
     *   @param[out] timeout     Timeout value in milliseconds
     *   @return Chimera::Status_t
     */
    virtual size_t getTimeout() = 0;

    /**
     *  Returns the max supported timeout by the hardware in milliseconds
     *
     *  @return size_t
     */
    virtual size_t maxTimeout() = 0;

    /**
     *  Returns the min supported timeout by the hardware in milliseconds
     *
     *  @return size_t
     */
    virtual size_t minTimeout() = 0;
  };
}  // namespace Chimera::Watchdog

#endif /* !CHIMERA_WATCHDOG_INTERFACE_HPP */