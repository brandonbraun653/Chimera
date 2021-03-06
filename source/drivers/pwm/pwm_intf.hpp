/********************************************************************************
 *  File Name:
 *    pwm_intf.hpp
 *
 *  Description:
 *    PWM Interface
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_PWM_INTERFACE_HPP
#define CHIMERA_PWM_INTERFACE_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/thread>
#include <Chimera/timer>

namespace Chimera::PWM
{
  namespace Backend
  {
    /**
     *  Registers the backend driver with Chimera
     *
     *  @param[in]  registry    Chimera's copy of the driver interface
     *  @return Chimera::Status_t
     */
    extern Chimera::Status_t registerDriver( DriverRegistration &registry );
  }  // namespace Backend

  class HardwareDriverInterface
  {
  public:
    virtual ~HardwareDriverInterface() = default;

    /**
     *  Initializes the PWM driver
     *
     *  @param[in]  cfg   The driver configuration settings
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t init( const DriverConfig &cfg ) = 0;

    /**
     *  Enables/Disables the PWM output signal on the configured GPIO pin
     *
     *  @param[in]  state   State to place the output in (on/off)
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t toggleOutput( const bool state ) = 0;

    /**
     *  Sets the output frequency of the PWM signal
     *
     *  @note Can be updated while the output is enabled
     *
     *  @warning  Any other channels associated with this one will
     *            have their frequency updated as well
     *
     *  @param[in]  freq    The new frequency to be set
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t setFrequency( const size_t freq ) = 0;

    /**
     *  Sets the output duty cycle of the PWM signal
     *
     *  @note Can be updated while the output is enabled
     *
     *  @param[in]  freq    The new frequency to be set
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t setDutyCyle( const size_t dutyCycle ) = 0;

    /**
     *  Sets the output polarity of the PWM signal
     *
     *  @note Can be updated while the output is enabled
     *
     *  @param[in]  polarity    The new polarity to be set
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t setPolarity( const Chimera::Timer::PWM::Polarity polarity ) = 0;
  };


  class IPWM : virtual public HardwareDriverInterface
  {
  public:
    virtual ~IPWM() = default;
  };
}  // namespace Chimera::PWM

#endif  /* !CHIMERA_PWM_INTERFACE_HPP */
