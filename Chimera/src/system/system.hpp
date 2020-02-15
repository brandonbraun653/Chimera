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
   *  Disables the current system interrupts
   *	@return void
   */
  void disableInterrupts();

  /**
   *  Restores previously disabled interrupts
   *	@return void
   */
  void enableInterrupts();

  /**
   *	Returns the maximum number of concurrent hardware threads that
   *  can be executing at any given time on the CPU.
   *	
   *	@return int
   */
  int maxConcurrentThreads();

}  // namespace Chimera::System

#endif /* !CHIMERA_SYSTEM_HPP */
