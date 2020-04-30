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

  ITimerEncoder_sPtr create_encoder_shared_ptr();
  ITimerEncoder_uPtr create_encoder_unique_ptr();

  ITimerInputCapture_sPtr create_input_capture_shared_ptr();
  ITimerInputCapture_uPtr create_input_capture_unique_ptr();

  ITimerOnePulse_sPtr create_one_pulse_shared_ptr();
  ITimerOnePulse_uPtr create_one_pulse_unique_ptr();

  ITimerOutputCompare_sPtr create_output_compare_shared_ptr();
  ITimerOutputCompare_uPtr create_output_compare_unique_ptr();

  ITimerPWM_sPtr create_pwm_shared_ptr();
  ITimerPWM_uPtr create_pwm_unique_ptr();

  size_t millis();

  void delayMilliseconds( const size_t val );

  void delayMicroseconds( const size_t val );
}

#endif  /* !CHIMERA_TIMER_HPP */
