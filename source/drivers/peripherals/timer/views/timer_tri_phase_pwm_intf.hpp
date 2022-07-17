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
#include <Chimera/source/drivers/peripherals/timer/views/timer_trigger_intf.hpp>

namespace Chimera::Timer::Inverter
{
  /*---------------------------------------------------------------------------
  Constants
  ---------------------------------------------------------------------------*/
  /**
   * @brief Total number of switches in the inverter
   */
  static constexpr size_t NUM_SWITCHES = 6;

  /*---------------------------------------------------------------------------
  Aliases
  ---------------------------------------------------------------------------*/
  /**
   * @brief Drive state of all switches in the inverter
   */
  using SwitchStates = std::array<Chimera::GPIO::State, NUM_SWITCHES>;

  /*---------------------------------------------------------------------------
  Enumerations
  ---------------------------------------------------------------------------*/
  /**
   * @brief Available switches in the 3-phase inverter
   */
  enum class SwitchId : uint8_t
  {
    HIGH_SIDE_PHASE_A,
    HIGH_SIDE_PHASE_B,
    HIGH_SIDE_PHASE_C,
    LOW_SIDE_PHASE_A,
    LOW_SIDE_PHASE_B,
    LOW_SIDE_PHASE_C,

    NUM_OPTIONS,
    INVALID
  };
  static_assert( EnumValue( SwitchId::NUM_OPTIONS ) == NUM_SWITCHES );

  /*---------------------------------------------------------------------------
  Structures
  ---------------------------------------------------------------------------*/
  /**
   * @brief Info to set up the timer with the appropriate configuration
   */
  struct DriverConfig
  {
    Chimera::Timer::CoreConfig      coreCfg;                      /**< Core timer configuration */
    Chimera::Timer::Output          pinMap[ NUM_SWITCHES ];       /**< Which channel each pin maps to */
    Chimera::Timer::Trigger::Signal adcTriggerSignal;             /**< Which signal the ADC is listening for a trigger on */
    Chimera::ADC::Peripheral        adcPeripheral;                /**< Which ADC to trigger for sampling */
    Chimera::GPIO::State            breakIOLevel;                 /**< Safe level for all IO on break */
    float                           pwmFrequency;                 /**< Desired frequency of the PWM output */
    float                           deadTimeNs;                   /**< Dead time between hi/lo complementary PWM */
    float                           adcTriggerOffsetNs;           /**< Offset from center of PWM ramp (center-aligned mode) */

    void clear()
    {
      coreCfg.clear();
      adcTriggerSignal   = Chimera::Timer::Trigger::Signal::TRIG_SIG_INVALID;
      adcPeripheral      = Chimera::ADC::Peripheral::UNKNOWN;
      adcTriggerOffsetNs = 0.0f;
      deadTimeNs         = 1.0f;
      pwmFrequency       = 1.0f;

      for ( size_t idx = 0; idx < NUM_SWITCHES; idx++ )
      {
        pinMap[ idx ] = Chimera::Timer::Output::INVALID;
      }
    }
  };

  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  /**
   * @brief 3-Phase Inverter PWM Controller
   * @warning Control functions likely called inside an ISR
   *
   * Timer controller implementation that generates complementary 3-phase PWM
   * output for the purpose of driving an Inverter. It's expected the controller
   * will be used in conjunction with some kind of external control loop to
   * modulate the 3-phase output to produce a desired waveform.
   *
   * Suggested External Resources:
   *  ADC - Used to sample phase current for feedback. Triggered by this Timer.
   *  ISR - Executes on DMA sample complete. Sets new phase duty cycles.
   */
  class Driver
  {
  public:
    Driver();
    ~Driver();

    /**
     * @brief Configures the timer to drive 3-phase PWM output
     * @note Assumes GPIO alternate functions remapped to TIMER externally
     *
     * @param cfg   Configuration settings
     * @return Chimera::Status_t
     */
    Chimera::Status_t init( const DriverConfig &cfg );

    /**
     * @brief Enables the PWM output signals on the configured GPIO pins
     * @return Chimera::Status_t
     */
    Chimera::Status_t enableOutput();

    /**
     * @brief Disables the PWM output signals on the configured GPIO pins
     * @return Chimera::Status_t
     */
    Chimera::Status_t disableOutput();

    /**
     * @brief Sets the core PWM frequency driving each output phase
     *
     * @param freq  Frequency to set in Hz
     * @return Chimera::Status_t
     */
    Chimera::Status_t setCarrierFrequency( const float freq );

    /**
     * @brief Sets the PWM carrier frequency duty cycle for each phase
     *
     * This generates an average voltage on the phase corresponding to the inverter
     * input DC voltage multiplied by the duty cycle. This is achieved by setting the
     * high side switch on-time percentage equal to the set duty cycle.
     *
     * For example:
     *  Duty Cycle: 20% => High Side On-Time: 20%, Low Side On-Time: 80%
     *
     * @param a   Phase A duty cycle from 0.0% to 100.0%
     * @param b   Phase B duty cycle from 0.0% to 100.0%
     * @param c   Phase C duty cycle from 0.0% to 100.0%
     * @return Chimera::Status_t
     */
    Chimera::Status_t setPhaseDutyCycle( const float a, const float b, const float c );

    /**
     * @brief Assign gating of the switches to enable/disable carrier PWM frequency drive
     *
     * Can be used by controllers to chop up the output drive. Most likely used to
     * implement motor commutation sequences.
     *
     * @param states  State of the inverter IO pins
     * @return Chimera::Status_t
     */
    Chimera::Status_t setSwitchGating( const SwitchStates &states );

    /**
     * @brief Quickly sets the output pins into a safe state
     *
     * @return Chimera::Status_t
     */
    Chimera::Status_t emergencyBreak();

  private:
    std::shared_ptr<void*> mTimerImpl;
  };

}  // namespace Chimera::Timer

#endif  /* !CHIMERA_TIMER_TRI_PHASE_PWM_HPP */
