/********************************************************************************
 *  File Name:
 *    sim_chimera_timer.hpp
 *
 *  Description:
 *    Simulator variant of the core chimera functionality.
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef SIM_CHIMERA_HPP
#define SIM_CHIMERA_HPP

#if defined( CHIMERA_SIMULATOR )

/* STL Includes */
#include <cstdlib>

/* Chimera Includes */
#include <Chimera/common>

namespace ChimeraSim::Timer
{
  Chimera::Status_t initialize();

  Chimera::Status_t reset();

  /**
   *	Returns the number of milliseconds that have elapsed since the beginning
   *  of the program.
   *
   *  @warning  Must call ChimeraInit() in order for this function to work
   *  @note     May not be exactly 1mS period as this is OS dependent
   *
   *	@return size_t
   */
  size_t millis();

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

}  // namespace ChimeraSim

#endif /* _WIN32 || _WIN64 */
#endif /* SIM_CHIMERA_HPP */