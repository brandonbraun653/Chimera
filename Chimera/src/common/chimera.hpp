/********************************************************************************
 *  File Name:
 *    chimera.hpp
 *
 *  Description:
 *    Generic header for frequently used Chimera functionality
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_HPP
#define CHIMERA_HPP

/* Chimera Includes */
#include <Chimera/src/common/common_types.hpp>
#include <Chimera/utility>

/**
 *	Initializes the Chimera subsystem. Should be implemented
 *  by the backend layer that hooks into Chimera.
 *
 *	@return void
 */
extern void ChimeraInit();

namespace Chimera
{
  /**
   *	Returns the number of milliseconds that have elapsed since the beginning
   *  of the program.
   *
   *  @warning  Must call ChimeraInit() in order for this function to work
   *  @note     May not be exactly 1mS resolution as this is OS dependent
   *
   *	@return size_t
   */
  size_t millis();

  /**
   *	Returns the number of microseconds that have elapsed since the beginning
   *  of the program.
   *
   *  @warning  Must call ChimeraInit() in order for this function to work
   *  @note     May not be exactly 1uS resolution as this is OS dependent
   *
   *	@return size_t
   */
  size_t micros();

  /**
   *  Delays (blocks) the current thread execution for a number of milliseconds
   *
   *  @note     May not be exact delays as this is OS dependent
   *
   *	@param[in]	val   Milliseconds to block the thread
   *	@return void
   */
  void delayMilliseconds( const size_t val );

  /**
   *  Delays (blocks) the current thread execution for a number of microseconds
   *
   *  @note     May not be exact delays as this is OS dependent
   *
   *	@param[in]	val   Microseconds to block the thread
   *	@return void
   */
  void delayMicroseconds( const size_t val );

}  // namespace Chimera

#endif
