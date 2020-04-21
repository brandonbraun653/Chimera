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
  PWM_sPtr create_shared_ptr();
  PWM_uPtr create_unique_ptr();
  size_t numSupportedChannels();
}  // namespace Chimera::PWM

#endif /* !CHIMERA_PWM_HPP */
