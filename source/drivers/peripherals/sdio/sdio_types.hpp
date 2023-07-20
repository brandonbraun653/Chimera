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
#include <Chimera/gpio>

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

  enum CallbackId : uint8_t
  {
    TX_COMPLETE,
    RX_COMPLETE,
    ERROR,
    ABORT,
    INITIALIZE,
    RESET,

    NUM_OPTIONS,
    CB_NUM_OPTIONS = NUM_OPTIONS,
    UNKNOWN
  };

  enum class BusWidth : uint8_t
  {
    BUS_WIDTH_1BIT,
    BUS_WIDTH_4BIT,

    NUM_OPTIONS,
    UNKNOWN
  };

  /*---------------------------------------------------------------------------
  Structures
  ---------------------------------------------------------------------------*/
  struct CardStatus
  {
    uint8_t  DataBusWidth;       /**< Shows the currently defined data bus width */
    uint8_t  SecuredMode;        /**< Card is in secured mode of operation */
    uint16_t CardType;           /**< Carries information about card type */
    uint32_t ProtectedAreaSize;  /**< Carries information about the capacity of protected area */
    uint8_t  SpeedClass;         /**< Carries information about the speed class of the card */
    uint8_t  PerformanceMove;    /**< Carries information about the card's performance move */
    uint8_t  AllocationUnitSize; /**< Carries information about the card's allocation unit size */
    uint16_t EraseSize;          /**< Determines the number of AUs to be erased in one operation */
    uint8_t  EraseTimeout;       /**< Determines the timeout for any number of AU erase */
    uint8_t  EraseOffset;        /**< Carries information about the erase offset */
  };

  struct CardIdentity
  {
    uint8_t  ManufacturerID; /**< Manufacturer ID */
    uint16_t OEM_AppliID;    /**< OEM/Application ID */
    uint32_t ProdName1;      /**< Product Name part1 */
    uint8_t  ProdName2;      /**< Product Name part2 */
    uint8_t  ProdRev;        /**< Product Revision */
    uint32_t ProdSN;         /**< Product Serial Number */
    uint16_t ManufactDate;   /**< Manufacturing Date */
  };

  struct CardSpecificData
  {
    uint8_t  CSDStruct;           /**< CSD structure */
    uint8_t  SysSpecVersion;      /**< System specification version */
    uint8_t  TAAC;                /**< Data read access time 1 */
    uint8_t  NSAC;                /**< Data read access time 2 in CLK cycles */
    uint8_t  MaxBusClkFrec;       /**< Max. bus clock frequency */
    uint16_t CardComdClasses;     /**< Card command classes */
    uint8_t  RdBlockLen;          /**< Max. read data block length */
    uint8_t  PartBlockRead;       /**< Partial blocks for read allowed */
    uint8_t  WrBlockMisalign;     /**< Write block misalignment */
    uint8_t  RdBlockMisalign;     /**< Read block misalignment */
    uint8_t  DSRImpl;             /**< DSR implemented */
    uint32_t DeviceSize;          /**< Device Size */
    uint8_t  MaxRdCurrentVDDMin;  /**< Max. read current @ VDD min */
    uint8_t  MaxRdCurrentVDDMax;  /**< Max. read current @ VDD max */
    uint8_t  MaxWrCurrentVDDMin;  /**< Max. write current @ VDD min */
    uint8_t  MaxWrCurrentVDDMax;  /**< Max. write current @ VDD max */
    uint8_t  DeviceSizeMul;       /**< Device size multiplier */
    uint8_t  EraseGrSize;         /**< Erase group size */
    uint8_t  EraseGrMul;          /**< Erase group size multiplier */
    uint8_t  WrProtectGrSize;     /**< Write protect group size */
    uint8_t  WrProtectGrEnable;   /**< Write protect group enable */
    uint8_t  ManDeflECC;          /**< Manufacturer default ECC */
    uint8_t  WrSpeedFact;         /**< Write speed factor */
    uint8_t  MaxWrBlockLen;       /**< Max. write data block length */
    uint8_t  WriteBlockPaPartial; /**< Partial blocks for write allowed */
    uint8_t  ContentProtectAppli; /**< Content protection application */
    uint8_t  FileFormatGroup;     /**< File format group */
    uint8_t  CopyFlag;            /**< Copy flag (OTP) */
    uint8_t  PermWrProtect;       /**< Permanent write protection */
    uint8_t  TempWrProtect;       /**< Temporary write protection */
    uint8_t  FileFormat;          /**< File format */
    uint8_t  ECC;                 /**< ECC code */
  };

  struct HWConfig
  {
    uint32_t               clockSpeed; /**< Clock speed of the SDIO bus */
    BusWidth               width;      /**< Number of data bits */
    Channel                channel;    /**< Which SDIO channel to use */
    Chimera::GPIO::PinInit cmdPin;     /**< Pin configuration for the command line */
    Chimera::GPIO::PinInit clkPin;     /**< Pin configuration for the clock line */
    Chimera::GPIO::PinInit dxPin[ 4 ]; /**< Pin configuration for the data line 0 */

    void clear()
    {
      clockSpeed = 0;
      width      = BusWidth::UNKNOWN;
      channel    = Channel::UNKNOWN;
      cmdPin     = Chimera::GPIO::PinInit();
      clkPin     = Chimera::GPIO::PinInit();
      dxPin[ 0 ] = Chimera::GPIO::PinInit();
      dxPin[ 1 ] = Chimera::GPIO::PinInit();
      dxPin[ 2 ] = Chimera::GPIO::PinInit();
      dxPin[ 3 ] = Chimera::GPIO::PinInit();
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
       *  Factory function that gets a pointer to an instance of the backend
       *  driver, as long as it conforms to the expected interface.
       */
      Driver_rPtr ( *getDriver )( const Channel periph );

      /**
       * @brief Clears the entire structure to default values
       */
      void clear()
      {
        isSupported = false;
        initialize  = nullptr;
        reset       = nullptr;
        getDriver   = nullptr;
      }
    };
  }  // namespace Backend
}  // namespace Chimera::SDIO

#endif /* !CHIMERA_SDIO_TYPES_HPP */
