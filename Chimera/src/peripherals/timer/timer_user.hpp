/********************************************************************************
 *  File Name:
 *    timer_user.hpp
 *
 *	Description:
 *    Chimera Timer
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_TIMER_HPP
#define CHIMERA_TIMER_HPP

/* STL Includes */
#include <cstdlib>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/src/peripherals/timer/timer_types.hpp>

namespace Chimera::Timer
{
  /**
   *  Initializes the timer module
   *
   *  @return Chimera::Status_t
   */
  Chimera::Status_t initialize();

  /**
   *  Resets the timer module
   */
  Chimera::Status_t reset();

  /**
   *  Returns the number of milliseconds elapsed since the beginning
   *  of program execution.
   *
   *  @return size_t
   */
  size_t millis();

  /**
   *  Returns the number of microseconds elapsed since the beginning
   *  of program execution.
   *
   *  @return size_t
   */
  size_t micros();

  /**
   *  Delays the current thread of execution a given number of milliseconds.
   *  If a scheduler is present, the delay will be non-blocking to other threads,
   *  otherwise the system will halt the specified amount.
   *
   *  @param[in]  val   The number of milliseconds to delay
   *  @return void
   */
  void delayMilliseconds( const size_t val );

  /**
   *  Delays the current thread of execution a given number of microseconds.
   *  If a scheduler is present, the delay will be non-blocking to other threads,
   *  otherwise the system will halt the specified amount.
   *
   *  @note Depending on the tick rate of the scheduler, non-blocking microsecond
   *        delays may not be possible.
   *
   *  @param[in]  val   The number of microseconds to delay
   *  @return void
   */
  void delayMicroseconds( const size_t val );

  /**
   *  Creates an unmanaged pointer to a timer instance
   *
   *  @warning Only use this function if you know what you are doing with raw pointers
   *
   *  @note This function is disabled unless the CHIMERA_DRIVER_INF_LIFETIME configuration
   *        option has been enabled.
   *
   *  @param[in]  peripheral    The peripheral to get the instance for
   *  @return ITimer_rPtr
   */
  ITimer_rPtr createUnsafeInstance( const Chimera::Timer::Peripheral peripheral );

  /**
   *  Creates a shared pointer to a timer instance
   *
   *  @param[in]  peripheral    The peripheral to get the instance for
   *  @return ITimer_sPtr
   */
  ITimer_sPtr createSharedInstance( const Chimera::Timer::Peripheral peripheral );

  /**
   *  Creates a unique pointer to a timer instance
   *
   *  @param[in]  peripheral    The peripheral to get the instance for
   *  @return ITimer_uPtr
   */
  ITimer_uPtr createUniqueInstance( const Chimera::Timer::Peripheral peripheral );
}

#endif  /* !CHIMERA_TIMER_HPP */
