/********************************************************************************
 *  File Name:
 *    system_intf.hpp
 *
 *  Description:
 *    Models the Chimera system interface
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SYSTEM_INTERFACE_HPP
#define CHIMERA_SYSTEM_INTERFACE_HPP

/* STL Includes */
#include <cstdint>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/src/system/system_types.hpp>

namespace Chimera::System
{
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
  extern Chimera::Status_t prjSystemStartup();

  /**
   *  Disables system level interrupts, thereby preventing any kind of interrupt from
   *  executing on the CPU.
   *
   *	@return size_t      Mask indicating which interrupts were disabled
   */
  extern Chimera::System::InterruptMask prjDisableInterrupts();

  /**
   *  Enables system level interrupts from the mask that was returned when
   *  interrupts were last disabled.
   *
   *	@return void
   */
  extern void prjEnableInterrupts( Chimera::System::InterruptMask &interruptMask );

  /**
   *	Returns the maximum number of concurrent hardware threads that
   *  can be executing at any given time on the CPU.
   *
   *	@return int
   */
  extern int prjMaxConcurrentThreads();

}  // namespace Chimera::System

#endif /* !CHIMERA_SYSTEM_INTERFACE_HPP */
