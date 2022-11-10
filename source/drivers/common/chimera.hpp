/********************************************************************************
 *  File Name:
 *    chimera.hpp
 *
 *  Description:
 *    Generic header for frequently used Chimera functionality
 *
 *  2019-2022 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_HPP
#define CHIMERA_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/source/drivers/common/common_types.hpp>
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
  /*---------------------------------------------------------------------------
  Constants
  ---------------------------------------------------------------------------*/
  static constexpr size_t DRIVER_INITIALIZED_KEY = static_cast<size_t>( 0x560bf751 );

  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  /**
   *  Returns the number of milliseconds that have elapsed since the beginning
   *  of the program.
   *
   *  @warning  Must call ChimeraInit() in order for this function to work
   *  @note     May not be exactly 1mS resolution as this is OS dependent
   *
   *  @return size_t
   */
  size_t millis();

  /**
   *  Returns the number of microseconds that have elapsed since the beginning
   *  of the program.
   *
   *  @warning  Must call ChimeraInit() in order for this function to work
   *  @note     May not be exactly 1uS resolution as this is OS dependent
   *
   *  @return size_t
   */
  size_t micros();

  /**
   *  Delays (blocks) the current thread execution until the requested system
   *  time has been reached. If that time has already passed, the function will
   *  return immediately.
   *
   *  @param[in]  val   System time in milliseconds
   *  @return void
   */
  void delayUntil( const size_t val );

  /**
   *  Delays (blocks) the current thread execution for a number of milliseconds
   *
   *  @note     May not be exact delays as this is OS dependent
   *
   *  @param[in]  val   Milliseconds to block the thread
   *  @return void
   */
  void delayMilliseconds( const size_t val );

  /**
   *  Delays (blocks) the current thread execution using an offset from the
   *  last time the thread woke up. This is intended to help create tasks that
   *  run at a specific period.
   *
   *  @code
   *  void SomeThread(void *arg)
   *  {
   *    size_t lastTimeWoken = 0;
   *    while(true)
   *    {
   *      lastTimeWoken = millis();
   *
   *      // Do some work that takes time
   *      doTimeConsumingWork();
   *
   *      // Delay until the tick given by "lastTimeWoken+50" is reached
   *      delayMilliseconds( lastTimeWoken, 50);
   *    }
   *  }
   *  @endcode
   *
   *  @param[in]  lastTimeWoken     Last time the thread started execution
   *  @param[in]  val               Offset to wake at in the future
   *  @return void
   */
  void delayMilliseconds( const size_t lastTimeWoken, const size_t val );

  /**
   *  Delays (blocks) the current thread execution for a number of microseconds
   *
   *  @note     May not be exact delays as this is OS dependent
   *
   *  @param[in]  val   Microseconds to block the thread
   *  @return void
   */
  void delayMicroseconds( const size_t val );

  /**
   *  Busy-wait delay for a number of milliseconds
   *
   *  @param[in]  val     Milliseconds to delay
   *  @return void
   */
  void blockDelayMilliseconds( const size_t val );

  /**
   *  Busy-wait delay for a number of microseconds.
   *
   *  @param[in]  val     Microseconds to delay
   *  @return void
   */
  void blockDelayMicroseconds( const size_t val );

}  // namespace Chimera

#endif
