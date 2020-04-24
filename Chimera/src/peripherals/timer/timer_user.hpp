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
  Chimera::Status_t initialize();

  Chimera::Status_t reset();

  ITimer_sPtr create_shared_ptr();

  ITimer_uPtr create_unique_ptr();

  size_t millis();

  void delayMilliseconds( const size_t val );

  void delayMicroseconds( const size_t val );
}

#endif  /* !CHIMERA_TIMER_HPP */
