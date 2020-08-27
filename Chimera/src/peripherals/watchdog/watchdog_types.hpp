/********************************************************************************
 *  File Name:
 *    watchdog_types.hpp
 *
 *  Description:
 *    Chimera Watchdog types
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_WATCHDOG_TYPES_HPP
#define CHIMERA_WATCHDOG_TYPES_HPP

/* STL Includes */
#include <cstdint>
#include <memory>

/* Chimera Includes */
#include <Chimera/common>

namespace Chimera::Watchdog
{
  /*-------------------------------------------------------------------------------
  Forward Declarations
  -------------------------------------------------------------------------------*/
  class IWatchdog;

  /*-------------------------------------------------------------------------------
  Aliases
  -------------------------------------------------------------------------------*/
  using IWatchdog_sPtr = std::shared_ptr<IWatchdog>;

  /*-------------------------------------------------------------------------------
  Enumerations
  -------------------------------------------------------------------------------*/
  enum Channel : uint8_t
  {
    WATCHDOG1,
    WATCHDOG2,

    NUM_OPTIONS
  };

  /*-------------------------------------------------------------------------------
  Structures
  -------------------------------------------------------------------------------*/
  namespace Backend
  {
    struct DriverConfig
    {
      bool isSupported; /**< A simple flag to let Chimera know if the driver is supported */

      /**
       *  Function pointer that initializes the backend driver's
       *  memory. Should really only call once for initial set up.
       */
      Chimera::Status_t ( *initialize )( void );

      /**
       *  Resets the backend driver hardware to default configuration
       *  settings, but does not wipe out any memory.
       */
      Chimera::Status_t ( *reset )(void );

      /**
       *  Factory function that creates a shared_ptr instance of the backend
       *  driver, as long as it conforms to the expected interface.
       */
      IWatchdog_sPtr ( *getDriver )( const Channel channel );
    };
  }  // namespace Backend
}  // namespace Chimera

#endif /* !CHIMERA_WATCHDOG_TYPES_HPP */