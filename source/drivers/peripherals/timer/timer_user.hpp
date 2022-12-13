/******************************************************************************
 *  File Name:
 *    timer_user.hpp
 *
 *	Description:
 *    Chimera timer user interface
 *
 *  2020-2022 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_TIMER_HPP
#define CHIMERA_TIMER_HPP

/* STL Includes */
#include <cstdlib>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/source/drivers/peripherals/timer/timer_common_types.hpp>

namespace Chimera::Timer
{
  /*---------------------------------------------------------------------------
  Public Functions
  ---------------------------------------------------------------------------*/
  Chimera::Status_t initialize();
  Chimera::Status_t reset();
  size_t            millis();
  size_t            micros();
  void              delayMilliseconds( const size_t val );
  void              delayMicroseconds( const size_t val );
  void              blockDelayMilliseconds( const size_t val );
  void              blockDelayMicroseconds( const size_t val );

}  // namespace Chimera::Timer

#endif /* !CHIMERA_TIMER_HPP */
