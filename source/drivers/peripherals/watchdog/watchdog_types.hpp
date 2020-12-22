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
  class IndependentDriver;
  class WindowDriver;

  /*-------------------------------------------------------------------------------
  Aliases
  -------------------------------------------------------------------------------*/
  using Independent_sPtr = std::shared_ptr<IndependentDriver>;
  using Window_sPtr = std::shared_ptr<WindowDriver>;

  /*-------------------------------------------------------------------------------
  Enumerations
  -------------------------------------------------------------------------------*/
  /**
   *  Channels for the independent watchdog timers
   */
  enum IChannel : uint8_t
  {
    WATCHDOG0,
    WATCHDOG1,

    NUM_OPTIONS,
    UNKNOWN
  };

  /**
   *  Channels for the windowed watchdog timers
   */
  enum WChannel : uint8_t
  {
    WATCHDOG0,
    WATCHDOG1,

    NUM_OPTIONS,
    UNKNOWN
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
       *  Gets the driver associated with the independent watchdog channel
       */
      Independent_sPtr ( *getIndependentDriver )( const IChannel channel );

      /**
       *  Gets the driver associated with the window watchdog channel
       */
      Window_sPtr ( *getWindowDriver )( const WChannel channel );
    };
  }  // namespace Backend
}  // namespace Chimera

#endif /* !CHIMERA_WATCHDOG_TYPES_HPP */