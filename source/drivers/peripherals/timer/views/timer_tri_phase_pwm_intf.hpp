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

  enum CommutationState : int
  {
    STATE_0,
    STATE_1,
    STATE_2,
    STATE_3,
    STATE_4,
    STATE_5,
    STATE_OFF,

    NUM_STATES,
    INVALID_STATE
  };

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
    Chimera::ADC::Peripheral        adcPeripheral;                /**< Which ADC to trigger for sampling */
    Chimera::GPIO::State            breakIOLevel;                 /**< Safe level for all IO on break */
    float                           pwmFrequency;                 /**< Desired frequency of the PWM output */
    float                           deadTimeNs;                   /**< Dead time between hi/lo complementary PWM */

    void clear()
    {
      coreCfg.clear();
      adcPeripheral      = Chimera::ADC::Peripheral::UNKNOWN;
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
   * output for the purpose of driving an BLDC motor using space vector modulation.
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
     * @brief Updates drive outputs using space vector modulation
     *
     * @param drive   Drive magnitude on the range of [0.0, 0.866]
     * @param theta   Electrical angle of the resulting drive vector in radians
     * @return Chimera::Status_t
     */
    Chimera::Status_t svmUpdate( const float drive, const float theta);

    /**
     * @brief Quickly sets the output pins into a safe state
     *
     * @return Chimera::Status_t
     */
    Chimera::Status_t emergencyBreak();

    /**
     * @brief Recomputes optimal trigger timing and minimum PWM period for safe operation.
     *
     * The goal of trigger timing is to align the ADC trigger with the PWM output so that
     * the ADC samples the current/voltage at the correct time. This function will compute
     * when to trigger the ADC so that the sample occurs at the center of the PWM output.
     *
     * @param adc_sample_time_ns  How long the ADC needs to perform all sample operations
     * @param trigger_offset_ns   Dead time between PWM output active and start of trigger output
     * @return Chimera::Status_t
     */
    Chimera::Status_t updateTriggerTiming( const uint32_t adc_sample_time_ns, const uint32_t trigger_offset_ns );

  private:
    void *mTimerImpl;
  };

}  // namespace Chimera::Timer

#endif  /* !CHIMERA_TIMER_TRI_PHASE_PWM_HPP */
