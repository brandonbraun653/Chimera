/******************************************************************************
 *  File Name:
 *    i2c_types.hpp
 *
 *  Description:
 *    Chimera I2C types
 *
 *  2021-2022 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_I2C_TYPES_HPP
#define CHIMERA_I2C_TYPES_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <cstdint>
#include <cstddef>
#include <Chimera/common>
#include <Chimera/gpio>

#if defined( SIMULATOR )
#include <filesystem>
#endif

namespace Chimera::I2C
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
    I2C1,
    I2C2,
    I2C3,
    I2C4,

    NUM_OPTIONS,
    NOT_SUPPORTED
  };

  enum class Frequency : uint8_t
  {
    F100KHZ,
    F400KHZ,

    NUM_OPTIONS,
    NOT_SUPPORTED
  };

  enum class Interrupt : uint8_t
  {
    START_TX,     /**< Start event sent */
    STOP_RX,      /**< Stop event received */
    TX_COMPLETE,  /**< Byte transfer completed */
    TX_EMPTY,     /**< Transmit buffer empty */
    RX_NOT_EMPTY, /**< Receive buffer not empty */

    NUM_OPTIONS,
    NOT_SUPPORTED
  };

  /*---------------------------------------------------------------------------
  Structures
  ---------------------------------------------------------------------------*/
  struct HardwareInit
  {
    Channel   channel;   /**< Hardware channel to configure */
    Frequency frequency; /**< Communication frequency */

    void clear()
    {
      channel   = Channel::NOT_SUPPORTED;
      frequency = Frequency::F100KHZ;
    }
  };

  struct DriverConfig
  {
    bool                   validity; /**< Defines if the configuration is valid */
    Chimera::GPIO::PinInit SDAInit;  /**< GPIO pin settings for SDA */
    Chimera::GPIO::PinInit SCLInit;  /**< GPIO pin settings for SCL */
    HardwareInit           HWInit;   /**< Hardware driver configuration options */

#if defined( SIMULATOR )
    std::filesystem::path memFile;  /**< Filesystem backing device */
    size_t                memSize;  /**< Required size of the file */
#endif

    void clear()
    {
      validity = false;
      SDAInit.clear();
      SCLInit.clear();
      HWInit.clear();
#if defined( SIMULATOR )
      memFile = "";
      memSize = 0;
#endif
    }
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
       *  Gets the driver instance associated with the requested channel
       */
      Driver_rPtr ( *getDriver )( const Channel channel );
    };

  }  // namespace Backend

}  // namespace Chimera::I2C

#endif /* !CHIMERA_I2C_TYPES_HPP */
