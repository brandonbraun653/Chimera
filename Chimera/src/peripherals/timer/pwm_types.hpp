/********************************************************************************
*   File Name:
*       pwm_types.hpp
*
*   Description:
*       Type specifications for the Chimera PWM module
*
*   2019 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/

#pragma once
#ifndef CHIMERA_PWM_TYPES_HPP
#define CHIMERA_PWM_TYPES_HPP

/* STL Includes */
#include <cstdint>

/* Chimera Includes */
#include <Chimera/types/gpio_types.hpp>

namespace Chimera::PWM
{
  /**
   *  Configuration structure for the PWMInterface 
   */ 
  struct Config
  {
    size_t channel;                   /**< The hardware PWM channel to use */
    size_t dutyCycle;                 /**< Initial duty cycle ([0-100])*/
    size_t period;                    /**< Initial period (uS) */
    Chimera::GPIO::PinInit gpioInit;  /**< Output GPIO configuration data */
  };

  /**
   *  Describes the supported PWM edge types
   */
  enum Edge : size_t 
  {
    RISING_EDGE,
    FALLING_EDGE,
    BOTH_EDGES
  };

  /**
   *  Function signature for edge notificatino callbacks
   */ 
  using EdgeCallback = void(*)(void);
}

#endif  /* CHIMERA_PWM_TYPES_HPP */