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

/* Chimera Includes */
#include <Chimera/types/common_types.hpp>

namespace Chimera
{
  namespace Watchdog
  {
    class Interface
    {
    public:
      virtual ~Interface() = default;

      /**
       *   Initializes the low level hardware needed to configure the watchdog
       * peripheral. This does not start the timer.
       *
       *   @note   Guarantees a minimum resolution of +/- 500uS around the specified
       * timeout
       *
       *   @param[in] timeout_mS        How many milliseconds can elapse before watchdog expires
       *   @param[in] windowPercent     Percentage away from timeout expiring before dog can be kicked
       *   @return Status::OK if the initialization was a success, Status::FAIL if
       * not
       */
      virtual Status_t initialize( const uint32_t timeout_mS, const uint8_t windowPercent ) = 0;

      /**
       *   Starts the watchdog timer. If successful, Interface::kick() must
       *   be called at regular intervals to prevent the watchdog from firing.
       *
       *   @return Peripheral status
       */
      virtual Status_t start() = 0;

      /**
       *   Stops the watchdog timer.
       *
       *   @return Peripheral status
       */
      virtual Status_t stop() = 0;

      /**
       *   Kicks the watchdog timer, starting a new countdown cycle.
       *
       *   @return Peripheral status
       */
      virtual Status_t kick() = 0;

      /**
       *   Gets the actual timeout value achieved by the hardware
       *
       *   @param[out] timeout     Timeout value in milliseconds
       *   @return Peripheral status
       */
      virtual Status_t getTimeout( uint32_t &timeout ) = 0;

      /**
       *   Configures the watchdog to stop on connection to a debugger
       *
       *   @param[in]  enable      If true, allows the watchdog to stop. Otherwise,
       * it continues running
       *   @return Peripheral status
       */
      virtual Status_t pauseOnDebugHalt( const bool enable ) = 0;
    };


    class WatchdogUnsupported : public Interface
    {
    public:
      virtual Status_t initialize( const uint32_t timeout_mS, const uint8_t windowPercent ) override
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }

      virtual Status_t start() override
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }

      virtual Status_t stop() override
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }

      virtual Status_t kick() override
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }

      virtual Status_t getTimeout( uint32_t &timeout ) override
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }

      virtual Status_t pauseOnDebugHalt( const bool enable ) override
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }
    };

  }  // namespace Watchdog
}  // namespace Chimera

#endif /* !CHIMERA_WATCHDOG_INTERFACE_HPP */