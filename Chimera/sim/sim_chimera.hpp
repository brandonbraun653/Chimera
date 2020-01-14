/********************************************************************************
 *   File Name:
 *       sim_chimera.hpp
 *
 *   Description:
 *       Simulator variant of the core chimera functionality.
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef SIM_CHIMERA_HPP
#define SIM_CHIMERA_HPP

/* C++ Includes */
#include <cstdint>

namespace ChimeraSim
{
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

#endif /* SIM_CHIMERA_HPP */