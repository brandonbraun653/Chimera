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
#include <Chimera/source/drivers/peripherals/gpio/gpio_types.hpp>
#include <Chimera/source/drivers/peripherals/timer/timer_common_types.hpp>
#include <Chimera/source/drivers/peripherals/timer/timer_intf.hpp>

namespace Chimera::Timer::PWM
{
  /*---------------------------------------------------------------------------
  Enumerations
  ---------------------------------------------------------------------------*/
  /**
   * @brief Output polarity of the PWM signal
   */
  enum class OutputPolarity
  {
    ACTIVE_HIGH,
    ACTIVE_LOW,

    NUM_OPTIONS
  };

  /**
   * @brief PWM generation mode
   */
  enum class GenerationMode
  {
    EDGE_ALIGNED,
    CENTER_ALIGNED,
    ASYMMETRIC,
    COMBINED,

    NUM_OPTIONS,
  };

  /*---------------------------------------------------------------------------
  Structures
  ---------------------------------------------------------------------------*/
  /**
   * @brief Configuration structure for initializing a PWM driver class
   */
  struct DriverConfig
  {
    bool                   allowPeriphReconfigure; /**< Allows the config to update shared channel register settings */
    Chimera::GPIO::PinInit outputPin;              /**< Output pin configuration */
    Chimera::GPIO::State   safeIOLevel;            /**< What output level to configure the GPIO to when disabling the I/O */
    Channel                outputChannel;          /**< Channel to configure the PWM output on */
    Instance               peripheral;             /**< Which peripheral to configure */
    OutputPolarity         polarity;               /**< Idle state when signal not asserted */
    float                  frequency;              /**< Desired frequency of the PWM output */
    float                  dutyCycle;              /**< Desired duty cycle 0-100% */

    void clear()
    {
      allowPeriphReconfigure = false;
      outputPin              = {};
      safeIOLevel            = Chimera::GPIO::State::LOW;
      outputChannel          = Channel::INVALID;
      peripheral             = Instance::NOT_SUPPORTED;
      polarity               = OutputPolarity::ACTIVE_HIGH;
      frequency              = 0.0f;
      dutyCycle              = 50.0f;
    }
  };


  /*---------------------------------------------------------------------------
  Class
  ---------------------------------------------------------------------------*/
  class Driver
  {
  public:

    static constexpr CoreFeature getInterfaceType()
    {
      return CoreFeature::PWM_OUTPUT;
    }

    static constexpr TimerInterface getClassType()
    {
      return TimerInterface::PWM_GENERATION;
    }


    Driver();
    ~Driver();

    /**
     * Initializes the PWM driver
     *
     * @param cfg   The driver configuration settings
     * @return Chimera::Status_t
     */
    Chimera::Status_t init( const DriverConfig &cfg );

    /**
     * Enables the PWM output signal on the configured GPIO pin
     * @return Chimera::Status_t
     */
    Chimera::Status_t enableOutput();

    /**
     * Enables the PWM output signal on the configured GPIO pin
     * @return Chimera::Status_t
     */
    Chimera::Status_t disableOutput();

    /**
     * Sets the output frequency of the PWM signal
     *
     * @note Can be updated while the output is enabled
     * @warning  Any other channels associated with this one will have their frequency updated as well
     *
     * @param freq    The new frequency to be set in Hz
     * @return Chimera::Status_t
     */
    Chimera::Status_t setFrequency( const size_t freq );

    /**
     * Sets the output duty cycle of the PWM signal
     *
     * @note Can be updated while the output is enabled
     *
     * @param freq    The new duty cycle to be set from 0-100
     * @return Chimera::Status_t
     */
    Chimera::Status_t setDutyCycle( const size_t dutyCycle );

    /**
     * Sets the output polarity of the PWM signal
     *
     * @note Can be updated while the output is enabled
     *
     * @param polarity    The new polarity to be set
     * @return Chimera::Status_t
     */
    Chimera::Status_t setPolarity( const Chimera::Timer::PWM::OutputPolarity polarity );

  private:
    void *mDriver; /**< Opaque pointer to the implementer's driver */
  };

}  // namespace Chimera::Timer

#endif  /* !CHIMERA_PWM_TIMER_INTERFACES_HPP */
