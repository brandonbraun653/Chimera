/********************************************************************************
 *   File Name:
 *    system_types.hpp
 *
 *   Description:
 *    Chimera System types
 *
 *   2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SYSTEM_TYPES_HPP
#define CHIMERA_SYSTEM_TYPES_HPP

/* STL Includes */
#include <cstdint>
#include <memory>

/* Chimera Includes */
#include <Chimera/common>

namespace Chimera::System
{
  /**
   *  The possible types of supported system reset events
   */
  enum class ResetEvent : uint8_t
  {
    BROWN_OUT,                       /**< Bad power condition*/
    EXIT_POWER_MODE,                 /**< Exiting some power mode (standby, shutdown, etc)*/
    SOFTWARE,                        /**< Software caused the reset*/
    HW_EXTERNAL_PIN,                 /**< Active logic level on the MCU reset pin */
    HW_INDEPENDENT_WATCHDOG_TIMEOUT, /**< Independent watchdog timed out*/
    HW_WINDOW_WATCHDOG_TIMEOUT,      /**< Window watchdog timed out*/
    UNKNOWN,                         /**< An undocumented reset event */
    CLEARED,                         /**< Hardware flags were cleared */
    NOT_SUPPORTED,                   /**< Reset reasoning logic isn't supported */
    MAX_RESET_TYPE
  };

  /**
   *  Physical hardware packaging option for this device
   */
  enum class Packaging : uint8_t
  {
    LQFP64,
    WLCSP64,
    LQFP100,
    LQFP144,
    WLCSP36,
    WLCSP81,
    UFQFPN32,
    LQFP32,
    UFQFPN48,
    LQFP48,
    WLCSP49,
    UFBGA64,
    UFBGA100,
    UNKNOWN
  };

  /**
   *  Encapsulates information about current system interrupt states
   */
  struct InterruptMask
  {
    bool interrupted;
    uint32_t mask;
  };

  /**
   *  System information that describes the chip that is running
   *  the host software. Intended to provide a generic overview
   *  of physical specs and capabilities.
   */
  struct Information
  {
    std::array<uint8_t, 12> uniqueId; /**< Unique identifier for this chip */
    size_t flashSize;                 /**< The amount of flash on the chip in kB */
    Packaging chipPackage;            /**< The chip's physical IC packaging type */
  };

  namespace Backend
  {
    /**
     *  Stores hooks for functions that the driver system should register with
     *  Chimera. Not all of the functions have to be implemented in order for
     *  the driver to be registered.
     *
     *  @note See function documentation in system_user.hpp
     */
    struct DriverConfig
    {
      bool isSupported; /**< A simple flag to let Chimera know if the driver is supported */

      Chimera::Status_t ( *initialize )( void );
      Chimera::Status_t ( *reset )( void );
      Chimera::Status_t ( *systemStartup )( void );
      Chimera::System::InterruptMask ( *disableInterrupts )( void );
      void ( *enableInterrupts )( Chimera::System::InterruptMask &interruptMask );
      int ( *maxConcurrentThreads )( void );
      ResetEvent ( *getResetReason )( void );
      bool ( *inISR )( void );
      void ( *getSystemInformation )( Information *&info );
      void ( *softwareReset )( void );
      std::string_view ( *version_AsString )( void );
      size_t ( *version_Major )( void );
      size_t ( *version_Minor )( void );
      size_t ( *version_Patch )( void );
      std::string_view ( *desc_About )( void );
      std::string_view ( *desc_BackendDriverName )( void );
      std::string_view ( *desc_DocumentationLink )( void );
    };
  }  // namespace Backend
}  // namespace Chimera::System

#endif /* !CHIMERA_SYSTEM_TYPES_HPP */