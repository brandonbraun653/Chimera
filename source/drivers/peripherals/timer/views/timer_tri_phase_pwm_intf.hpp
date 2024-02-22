/******************************************************************************
 *  File Name:
 *    timer_tri_phase_pwm_intf.hpp
 *
 *  Description:
 *     3-Phase PWM Controller for Motor Control Applications
 *
 *  2022-2024 | Brandon Braun | brandonbraun653@protonmail.com
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
  Enumerations
  ---------------------------------------------------------------------------*/

  /**
   * @brief Enumerate possible IO switches that can be controlled
   */
  enum SwitchIO : size_t
  {
    SWITCH_1_HI,
    SWITCH_1_LO,
    SWITCH_2_HI,
    SWITCH_2_LO,
    SWITCH_3_HI,
    SWITCH_3_LO,

    NUM_SWITCHES,
    SWITCH_INVALID
  };

  /*---------------------------------------------------------------------------
  Structures
  ---------------------------------------------------------------------------*/

  /**
   * @brief Info to set up the timer with the appropriate configuration
   */
  struct DriverConfig
  {
    Chimera::Timer::CoreConfig coreCfg;                /**< Core timer configuration */
    Chimera::Timer::Output     pinMap[ NUM_SWITCHES ]; /**< Which channel each pin maps to */
    Chimera::GPIO::State       breakIOLevel;           /**< Safe level for all IO on break */
    float                      pwmFrequency;           /**< Desired frequency of the PWM output */
    float                      deadTimeNs;             /**< Dead time between hi/lo complementary PWM */
    float                      settleTimeNs;           /**< Time to wait for phase currents to settle after a switch event */
    float                      adcSampleTimeNs;        /**< Window needed for ADC to complete all samples */

    void clear()
    {
      coreCfg.clear();
      deadTimeNs   = 1.0f;
      pwmFrequency = 1.0f;
      settleTimeNs = 1.0f;

      for( size_t idx = 0; idx < NUM_SWITCHES; idx++ )
      {
        pinMap[ idx ] = Chimera::Timer::Output::INVALID;
      }
    }
  };

  /**
   * @brief Computed outputs of the SVM algorithm
   */
  struct SVMState
  {
    bool                    adc_polarity; /**< Polarity to trigger ADC on. True => Rising, False => Falling*/
    Chimera::Timer::Channel phase1;       /**< Timer channel for best phase 1 current */
    Chimera::Timer::Channel phase2;       /**< Timer channel for best phase 2 current */
    uint32_t                t1_cc;        /**< Timer compare value for channel 1 */
    uint32_t                t2_cc;        /**< Timer compare value for channel 2 */
    uint32_t                t3_cc;        /**< Timer compare value for channel 3 */
    uint32_t                t4_cc;        /**< Timer compare value for channel 4 */
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
     * @brief Resets the timer to a known idle state
     * @return void
     */
    void reset();

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
     * @brief Energizes the winding configured by the given switch IOs.
     *
     * This will drive the IO pins using a duty cycle of the given value. Internal
     * protections are in place to ensure a valid combination of IOs are used. Once
     * enabled, the PWM output will remain active until disableOutput() is called.
     *
     * @param hiSide Switch to connect to Vbus
     * @param loSide Switch to connect to ground
     * @param dutyCycle PWM duty cycle to use
     * @return Chimera::Status_t
     */
    Chimera::Status_t energizeWinding( const SwitchIO hiSide, const SwitchIO loSide, const float dutyCycle );

    /**
     * @brief Updates drive outputs using space vector modulation
     *
     * @param alpha   Output of inverse park transform
     * @param beta    Output of inverse park transform
     * @param theta   Desired electrical angle of the resulting drive vector in radians
     * @param drive   Modulation index of the drive vector, ranged [0, pi/3]
     * @return Chimera::Status_t
     */
    Chimera::Status_t svmUpdate( const float alpha, const float beta, const float theta, const float drive );

    /**
     * @brief Gets the current decision outputs of the svm update algorithm
     * @return void
     */
    SVMState svmState();

    /**
     * @brief Quickly sets the output pins into a safe state
     *
     * @return Chimera::Status_t
     */
    Chimera::Status_t emergencyBreak();

  private:
    void *mTimerImpl;
    Chimera::Status_t reset_to_idle();
  };

}  // namespace Chimera::Timer::Inverter

#endif /* !CHIMERA_TIMER_TRI_PHASE_PWM_HPP */
