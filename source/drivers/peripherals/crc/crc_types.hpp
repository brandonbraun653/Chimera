/******************************************************************************
 *  File Name:
 *    crc_types.hpp
 *
 *  Description:
 *    Chimera CRC types
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_CRC_TYPES_HPP
#define CHIMERA_CRC_TYPES_HPP

/* STL Includes */
#include <cstdint>
#include <memory>

/* Chimera Includes */
#include <Chimera/common>

namespace Chimera::CRC
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
    CHANNEL1,
    CHANNEL2,

    NUM_OPTIONS
  };

  /*---------------------------------------------------------------------------
  Structures
  ---------------------------------------------------------------------------*/
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
      Chimera::Status_t ( *reset )( void );

      /**
       *  Factory function that gets a pointer to an instance of the backend
       *  driver, as long as it conforms to the expected interface.
       */
      Driver_rPtr ( *getDriver )( const Channel channel );
    };
  }  // namespace Backend
}  // namespace Chimera::CRC

#endif /* !CHIMERA_CRC_TYPES_HPP */