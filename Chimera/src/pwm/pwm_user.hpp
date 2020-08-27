/********************************************************************************
 *  File Name:
 *    pwm.hpp
 *
 *  Description:
 *    PWM user interface
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_PWM_HPP
#define CHIMERA_PWM_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/src/pwm/pwm_types.hpp>

namespace Chimera::PWM
{
  Chimera::Status_t initialize();
  Chimera::Status_t reset();
  IPWM_sPtr getDriver( const size_t channel );
  size_t numSupportedChannels();
}  // namespace Chimera::PWM

#endif /* !CHIMERA_PWM_HPP */
