/********************************************************************************
 *  File Name:
 *    pwm_types.hpp
 *
 *  Description:
 *    PWM Types
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_PWM_TYPES_HPP
#define CHIMERA_PWM_TYPES_HPP

/* STL Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/gpio>
#include <Chimera/timer>

namespace Chimera::PWM
{
  /**
   *  Initialization structure to configure a PWM driver
   */
  struct DriverConfig
  {
    Chimera::GPIO::PinInit outputPin;          /**< Output pin configuration */
    Chimera::Timer::PWM::Config pwm;           /**< PWM configuration */
    Chimera::Timer::DriverConfig timer;        /**< Optional timer hardware config */
    bool validity;                             /**< Is this configuration data valid */
  };

  class IPWM;
  using PWM_sPtr = std::shared_ptr<IPWM>;
  using PWM_uPtr = std::unique_ptr<IPWM>;

  namespace Backend
  {
    struct DriverRegistration
    {
      bool isSupported;

      /**
       *  Initializes the backend driver memory
       *
       *  @return Chimera::Status_t
       */
      Chimera::Status_t ( *initialize )( void );

      /**
       *  Resets the backend driver, bringing it to a state that appears
       *  as if the MCU just powered up.
       *
       *  @return Chimera::Status_t
       */
      Chimera::Status_t ( *reset )( void );

      /**
       *  Creates a shared driver object
       *
       *  @return PWM_sPtr
       */
      PWM_sPtr ( *createShared )( void );

      /**
       *  Creates a unique driver object
       *
       *  @return PWM_uPtr
       */
      PWM_uPtr ( *createUnique )( void );

      /**
       *  Gets the number of supported PWM channels on the device
       *
       *  @return size_t
       */
      size_t ( *numSupportedChannels )( void );
    };
  }
}  // namespace Chimera::PWM

#endif  /* !CHIMERA_PWM_TYPES_HPP */
