/******************************************************************************
 *  File Name:
 *    system.hpp
 *
 *  Description:
 *    Provides common system level functions
 *
 *  2019-2021 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_SYSTEM_HPP
#define CHIMERA_SYSTEM_HPP

/* STL Includes */
#include <cstdint>
#include <string>
#include <memory>
#include <type_traits>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/source/drivers/system/system_types.hpp>

namespace Chimera::System
{
  /**
   *  Performs core system initialization functionality for both Chimera
   *  and the attached backend driver. Once this function exits, it is
   *  assumed that the MCU is completely initialized and ready to execute
   *  user code.
   *
   *  @warning Must be the first call upon entry into user code (typically main())
   *
   *  @return Chimera::Status_t
   */
  Chimera::Status_t initialize();

  /**
   *  Low level driver specific system initialization function used for operations
   *  like clock configuration, peripheral startup, etc.
   *
   *  @note     Intended to allow the backend driver to initialize itself before Chimera
   *            code begins execution.
   *
   *  @warning  Chimera assumes that once this function exits, the backend driver is ready
   *            to be used in its entirety.
   *
   *  @return Chimera::Status_t
   */
  Chimera::Status_t systemStartup();

  /**
   *  Disables all interrupts and returns a mask indicating which
   *  ISR vector signals should be unmasked in enableInterrupts()
   *
   *	@return Chimera::System::InterruptMask
   */
  Chimera::System::InterruptMask disableInterrupts();

  /**
   *  Restores previously disabled interrupts
   *
   *  @param[in]  interruptMask     Mask returned from disableInterrupts()
   *	@return void
   */
  void enableInterrupts( Chimera::System::InterruptMask &interruptMask );

  /**
   *	Returns the maximum number of concurrent hardware threads that
   *  can be executing at any given time on the CPU.
   *
   *	@return int
   */
  int maxConcurrentThreads();

  /**
   *  Returns the last known reason why the hardware was reset
   *
   *  @return ResetEvent
   */
  ResetEvent getResetReason();

  /**
   *  Checks if the processor is currently executing inside an ISR handler
   *
   *  @return bool
   */
  bool inISR();

  /**
   *  Gets high level system information that describes this chip
   *
   *  @param[in]  info    Chimera owned pointer for backend to point at local memory
   *  @return void
   */
  void getSystemInformation( Information *&info );

  /**
   *  Instructs the processor to issue a reset command. The effect is the
   *  same as if the hardware RESET pin was asserted.
   *
   *  @return void
   */
  void softwareReset();

  /**
   * @brief Checks if a debugger is attached
   *
   * @return true
   * @return false
   */
  bool isDebuggerAttached();


  namespace Version
  {
    /**
     *  Gets the Chimera driver version information as a string
     *  @return std::string_view
     */
    std::string_view asString();

    /**
     *  Gets the major version number of the backend driver
     *  @return size_t
     */
    size_t major();

    /**
     *  Gets the minor version number of the backend driver
     *  @return size_t
     */
    size_t minor();

    /**
     *  Gets the patch version number of the backend driver
     *  @return size_t
     */
    size_t patch();
  }  // namespace Version

  namespace Description
  {
    /**
     *  Describes the backend driver with whatever information is desired
     *  @return std::string_view
     */
    std::string_view about();

    /**
     *  Name of the backend driver
     *  @return std::string_view
     */
    std::string_view backendDriverName();

    /**
     *  Link to where documentation for the backend driver should be
     *  @return std::string_view
     */
    std::string_view documentationLink();
  }  // namespace Description

}  // namespace Chimera::System

#endif /* !CHIMERA_SYSTEM_HPP */
