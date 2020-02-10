/********************************************************************************
*   File Name:
*       pwm_intf.hpp
*
*   Description:
*       Module interface description for the Chimera PWM driver
*
*   2019 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/

#pragma once
#ifndef CHIMERA_PWM_INTERFACE_HPP
#define CHIMERA_PWM_INTERFACE_HPP

/* C++ Includes */
#include <cstdint>

/* Chimera Includes */
#include <Chimera/types/common_types.hpp>
#include <Chimera/types/pwm_types.hpp>

namespace Chimera::PWM
{
  class PWMInterface
  {
    public:
      virtual ~PWMInterface() = default;

      /**
       *  Initializes the PWM driver with the configuration settings
       *  specified. Leaves the hardware in an idle state until enabled. 
       *  
       *  @note   Cannot be reinitalized until deInit() is called first
       *  
       *  @param[in]  cfg     The configuration structure
       *  @return Chimera::Status_t
       * 
       *  |  Return Value |                    Explanation                    |
       *  |:-------------:|:-------------------------------------------------:|
       *  |            OK | Everything initialized successfully               |
       *  |          FAIL | Could not initialize the hardware                 |
       *  | NOT_SUPPORTED | One of the configuration options is not supported |
       */
      virtual Chimera::Status_t init( const Config *const cfg ) = 0;

      /**
       *  Deinitializes the PWM driver
       *  
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t deInit() = 0;

      /**
       *  Dynamically updates the duty cycle of the output at runtime
       * 
       *  @note   Performs the update without glitching
       *  
       *  @param[in]  dutyCycle   Desired duty cycle on the range [0, 100]
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t setDutyCycle( const size_t dutyCycle ) = 0;

      /**
       *  Dynamically updates the period of the output at runtime
       * 
       *  @note   Performs the update without glitching
       * 
       *  @param[in]  period      Desired period in microseconds on the range [1, x)
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t setPeriod( const size_t period ) = 0;

      /**
       *  Enables or disables the waveform output on the configured GPIO pin
       * 
       *  @param[in]  state       Enables (true) or Disables (false) the output
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t toggleOutput( const bool state ) = 0;

      /**
       *  Enables or disables processing of an edge event callback
       * 
       *  @param[in]  edge        The edge trigger callback to enable/disable
       *  @param[in]  state       Set as enabled/disabled
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t toggleEdgeNotification( const Edge edge, const bool state ) = 0;

      /**
       *  Attaches a callback to be invoked upon a given edge event.
       *  
       *  @note   By default the event processing is disabled. To enable, call toggleEdgeNotification()
       * 
       *  @param[in]  edge        The edge trigger to attach the callback to
       *  @param[in]  callback    The callback function
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t attachEdgeNotification( const Edge edge, EdgeCallback callback ) = 0;
  };
}

#endif  /* CHIMERA_PWM_INTERFACE_HPP */
