/******************************************************************************
 *  File Name:
 *    sdio_types.hpp
 *
 *  Description:
 *    Types for the Chimera SDIO driver
 *
 *  2023 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_SDIO_TYPES_HPP
#define CHIMERA_SDIO_TYPES_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/common>

namespace Chimera::SDIO
{
  /*---------------------------------------------------------------------------
  Forward Declarations
  ---------------------------------------------------------------------------*/
  class Driver;

  /*---------------------------------------------------------------------------
  Aliases
  ---------------------------------------------------------------------------*/
  using Driver_rPtr = Driver *;

  /*---------------------------------------------------------------------------
  Enumerations
  ---------------------------------------------------------------------------*/
  enum class Channel : uint8_t
  {
    SDIO1,
    SDIO2,

    NUM_OPTIONS,
    UNKNOWN
  };

  /*---------------------------------------------------------------------------
  Structures
  ---------------------------------------------------------------------------*/
	struct HWConfig
	{

	};

  namespace Backend
  {
    struct DriverConfig
    {
      bool isSupported; /**< A simple flag to let Chimera know if the driver is supported */

      /**
       *  Function pointer that initializes the backend driver's
       *  memory. Should really only call once for initial set up.
       */
      Chimera::Status_t ( *initialize )();

      /**
       *  Resets the backend driver hardware to default configuration
       *  settings, but does not wipe out any memory.
       */
      Chimera::Status_t ( *reset )();

      /**
       *  Factory function that gets a pointer to an instance of the backend
       *  driver, as long as it conforms to the expected interface.
       */
      Driver_rPtr ( *getDriver )( const Channel periph );
    };
  }   // namespace Backend
}  // namespace Chimera::SDIO

#endif /* !CHIMERA_SDIO_TYPES_HPP */
