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
  class IWatchdog;

  using Watchdog_sPtr = std::shared_ptr<IWatchdog>;
  using Watchdog_uPtr = std::unique_ptr<IWatchdog>;

  namespace Backend
  {
    using Initialize_FPtr         = Chimera::Status_t ( * )( void );
    using Reset_FPtr              = Chimera::Status_t ( * )( void );
    using CreateSharedObject_FPtr = Watchdog_sPtr ( * )( void );
    using CreateUniqueObject_FPtr = Watchdog_uPtr ( * )( void );

    struct DriverConfig
    {
      bool isSupported; /**< A simple flag to let Chimera know if the driver is supported */

      /**
       *  Function pointer that initializes the backend driver's
       *  memory. Should really only call once for initial set up.
       */
      Initialize_FPtr initialize;

      /**
       *  Resets the backend driver hardware to default configuration
       *  settings, but does not wipe out any memory.
       */
      Reset_FPtr reset;

      /**
       *  Factory function that creates a shared_ptr instance of the backend
       *  driver, as long as it conforms to the expected interface.
       */
      CreateSharedObject_FPtr createShared;

      /**
       *  Factory function that creates a unique_ptr instance of the backend
       *  driver, as long as it conforms to the expected interface.
       */
      CreateUniqueObject_FPtr createUnique;
    };
  }  // namespace Backend
}  // namespace Chimera

#endif /* !CHIMERA_WATCHDOG_TYPES_HPP */