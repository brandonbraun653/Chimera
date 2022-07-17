/******************************************************************************
 * File Name:
 *   timer_base_intf.hpp
 *
 * Description:
 *   Interface classes for base time unit classes
 *
 * 2022 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_PWM_TIMER_INTERFACES_HPP
#define CHIMERA_PWM_TIMER_INTERFACES_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <memory>
#include <Chimera/source/drivers/peripherals/gpio/gpio_types.hpp>
#include <Chimera/source/drivers/peripherals/timer/timer_common_types.hpp>
#include <Chimera/source/drivers/peripherals/timer/timer_intf.hpp>
#include <Chimera/source/drivers/peripherals/timer/views/timer_base_intf.hpp>

namespace Chimera::Timer::PWM
{
  /*---------------------------------------------------------------------------
  Enumerations
  ---------------------------------------------------------------------------*/
  /**
   * @brief Output polarity of the PWM signal
   */
  enum class Polarity
  {
    ACTIVE_HIGH,
    ACTIVE_LOW,

    NUM_OPTIONS
  };

  /*---------------------------------------------------------------------------
  Structures
  ---------------------------------------------------------------------------*/
  /**
   * @brief Configuration structure for initializing a PWM driver class
   */
  struct DriverConfig
  {
    Chimera::Timer::CoreConfig coreCfg;     /**< Core timer configuration */
    float                      dutyCycle;   /**< Desired duty cycle 0-100% */
    float                      frequency;   /**< Desired frequency of the PWM output */
    Channel                    channel;     /**< Timer channel to configure the PWM output on */
    Output                     output;      /**< Timer output selection */
    Chimera::GPIO::PinInit     gpioPin;     /**< GPIO output pin configuration */
    Polarity                   polarity;    /**< Idle state when signal not asserted */
    Chimera::GPIO::State       safeIOLevel; /**< What output level to configure the GPIO to when disabling the I/O */

    void clear()
    {
      dutyCycle   = 50.0f;
      frequency   = 0.0f;
      channel     = Channel::INVALID;
      output      = Output::INVALID;
      polarity    = Polarity::ACTIVE_HIGH;
      safeIOLevel = Chimera::GPIO::State::LOW;

      coreCfg.clear();
      gpioPin.clear();
    }
  };


  /*---------------------------------------------------------------------------
  Class
  ---------------------------------------------------------------------------*/
  /**
   * @brief A basic PWM class
   *
   * Does exactly what you'd expect. There are no fancy configurations or
   * special fields to be set, just a plain 'ol PWM signal on a GPIO pin.
   */
  class Driver
  {
  public:
    Driver();
    ~Driver();

    /**
     * @brief Initializes the PWM driver
     *
     * @param cfg   The driver configuration settings
     * @return Chimera::Status_t
     */
    Chimera::Status_t init( const DriverConfig &cfg );

    /**
     * @brief Enables the PWM output signal on the configured GPIO pin
     * @return Chimera::Status_t
     */
    Chimera::Status_t enableOutput();

    /**
     * @brief Disables the PWM output signal on the configured GPIO pin
     * @return Chimera::Status_t
     */
    Chimera::Status_t disableOutput();

    /**
     * @brief Sets the output frequency of the PWM signal
     *
     * @note Can be updated while the output is enabled
     * @warning  Any other channels associated with this one will have their frequency updated as well
     *
     * @param freq    The new frequency to be set in Hz
     * @return Chimera::Status_t
     */
    Chimera::Status_t setFrequency( const float freq );

    /**
     * @brief Sets the output duty cycle of the PWM signal
     *
     * @note Can be updated while the output is enabled
     *
     * @param freq    The new duty cycle to be set from 0-100
     * @return Chimera::Status_t
     */
    Chimera::Status_t setDutyCycle( const float dutyCycle );

    /**
     * @brief Sets the output polarity of the PWM signal
     *
     * @note Can be updated while the output is enabled
     *
     * @param polarity    The new polarity to be set
     * @return Chimera::Status_t
     */
    Chimera::Status_t setPolarity( const Chimera::Timer::PWM::Polarity polarity );


    static constexpr CoreFeature getInterfaceType()
    {
      return CoreFeature::PWM_OUTPUT;
    }

    static constexpr TimerInterface getClassType()
    {
      return TimerInterface::PWM_GENERATION;
    }

  private:
    std::shared_ptr<void *> mTimerImpl;
  };

}  // namespace Chimera::Timer::PWM

#endif /* !CHIMERA_PWM_TIMER_INTERFACES_HPP */
