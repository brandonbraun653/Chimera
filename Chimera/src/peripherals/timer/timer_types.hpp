/********************************************************************************
 *  File Name:
 *    timer_types.hpp
 *
 *	 Description:
 *    Types for the Chimera Timer module
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_TIMER_TYPES_HPP
#define CHIMERA_TIMER_TYPES_HPP

/* STL Includes */
#include <cstdlib>
#include <memory>

/* Chimera Includes */
#include <Chimera/common>

namespace Chimera::Timer
{
  class ITimer;
  using ITimer_sPtr = std::shared_ptr<ITimer>;
  using ITimer_uPtr = std::unique_ptr<ITimer>;

  enum class Direction
  {
    COUNT_UP,     /**< Counts up from min, overflows, then starts counting up again */
    COUNT_DN,     /**< Counts down from max, underflows, then starts counting down again */
    COUNT_UP_DN,  /**< Alternates between counting up, down, up, etc */

    NUM_OPTIONS
  }; 
  
  enum class Mode
  {
    INPUT_CAPTURE,
    OUTPUT_COMPARE,
    PWM_OUTPUT,
    ONE_PULSE_OUTPUT,

    NUM_OPTIONS
  };

  enum class ISREvent
  {
    // Fill in when needed
    NUM_OPTIONS
  };

  struct DriverConfig
  {
    bool validity;            /**< Decides if the configuration settings are valid */
    bool overwrite;           /**< Allows the config to update the entire timer peripheral (multiple channels share one peripheral) */
    Direction countDirection; /**< Which direction the free-running counter should count */
    size_t reloadValue;       /**< Value to load when the counter overflows */
    size_t channel;           /**< Output channel this driver is controlling */
  };

  namespace InputCapture
  {
    struct Config
    {
      // Currently not used
    };
  }

  namespace OutputCompare
  {
    struct Config
    {
      // Currently not used
    };
  }

  namespace PWMGeneration
  {
    enum class Polarity
    {
      ACTIVE_HIGH,
      ACTIVE_LOW,

      NUM_OPTIONS
    };

    struct Config
    {
      // Currently not used
    };
  }

  namespace OnePulse
  {
    struct Config
    {
      // Currently not used
    };
  }

  namespace Backend
  {
    struct DriverRegistration
    {
      bool isSupported;
      Chimera::Status_t ( *initialize )( void );
      Chimera::Status_t ( *reset )( void );
      ITimer_sPtr ( *create_shared_ptr )( void );
      ITimer_uPtr ( *create_unique_ptr )( void );
      size_t ( *millis )( void );
      void ( *delayMilliseconds )( const size_t );
      void ( *delayMicroseconds )( const size_t );
    };
  }  // namespace Backend
}

#endif  /* !CHIMERA_TIMER_TYPES_HPP */
