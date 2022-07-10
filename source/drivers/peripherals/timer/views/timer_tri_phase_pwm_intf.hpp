/******************************************************************************
 *  File Name:
 *    timer_tri_phase_pwm_intf.hpp
 *
 *  Description:
 *     3-Phase PWM Controller for Motor Control Applications
 *
 *  2022 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_TIMER_TRI_PHASE_PWM_HPP
#define CHIMERA_TIMER_TRI_PHASE_PWM_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <memory>
#include <Chimera/source/drivers/peripherals/adc/adc_types.hpp>
#include <Chimera/source/drivers/peripherals/gpio/gpio_types.hpp>
#include <Chimera/source/drivers/peripherals/timer/timer_common_types.hpp>
#include <Chimera/source/drivers/peripherals/timer/timer_intf.hpp>
#include <Chimera/source/drivers/peripherals/timer/views/timer_base_intf.hpp>

namespace Chimera::Timer::TriPhasePWM
{
  /*---------------------------------------------------------------------------
  Enumerations
  ---------------------------------------------------------------------------*/

  /*---------------------------------------------------------------------------
  Structures
  ---------------------------------------------------------------------------*/
  struct DriverConfig
  {
    bool complementaryOutput;
    float deadTime_ns;

    // Commutation table?

    Chimera::ADC::Peripheral adcTrigger;  /**< Which ADC to trigger for sampling */

    void clear()
    {
      complementaryOutput = false;
      deadTime_ns         = 1.0f;
    }
  };

  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  /**
   * @brief 3-Phase PWM Controller
   *
   * This implementation is a bit biased towards a particular kind of controller:
   *  -
   */
  class Driver
  {
  public:

    Chimera::Status_t init( const DriverConfig &cfg );

    Chimera::Status_t setFrequency( const float freq );

    Chimera::Status_t setPhaseDutyCycle( const float a, const float b, const float c );

    Chimera::Status_t setElectricalAngle( const uint32_t deg );

    Chimera::Status_t emergencyBreak();

  private:
    std::shared_ptr<void*> mTimerImpl;
  };

}  // namespace Chimera::Timer

#endif  /* !CHIMERA_TIMER_TRI_PHASE_PWM_HPP */
