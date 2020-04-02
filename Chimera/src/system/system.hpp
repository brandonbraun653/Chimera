/********************************************************************************
 *  File Name:
 *    system.hpp
 *
 *  Description:
 *    Provides common system level functions
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SYSTEM_HPP
#define CHIMERA_SYSTEM_HPP

/* STL Includes */
#include <cstdint>
#include <memory>
#include <type_traits>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/src/system/system_types.hpp>

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
   *  Disables the current system interrupts
   *	@return void
   */
  Chimera::System::InterruptMask disableInterrupts();

  /**
   *  Restores previously disabled interrupts
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

  void getSystemInformation( Information *&info );

}  // namespace Chimera::System

#endif /* !CHIMERA_SYSTEM_HPP */
