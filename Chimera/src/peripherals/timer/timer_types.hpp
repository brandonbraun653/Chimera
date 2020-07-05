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
  using ITimer_rPtr = ITimer *;
  using ITimer_sPtr = std::shared_ptr<ITimer>;
  using ITimer_uPtr = std::unique_ptr<ITimer>;

  /*-------------------------------------------------------------------------------
  Enumerations
  -------------------------------------------------------------------------------*/
  enum class CoreFeature : uint8_t
  {
    BASE_TIMER,
    INPUT_CAPTURE,
    OUTPUT_COMPARE,
    PWM_OUTPUT,
    ONE_PULSE_OUTPUT,
    ENCODER,

    NUM_OPTIONS,
    INVALID
  };

  enum class DriverAction : size_t
  {
    PWM_ACTION_BEGIN,
    PWM_DISABLE_CHANNEL = PWM_ACTION_BEGIN, /**< Disables a PWM channel output */
    PWM_ENABLE_CHANNEL,                     /**< Enables a PWM channel output */
    PWM_SET_DUTY_CYCLE,                     /**< Updates the duty cycle of a PWM channel */
    PWM_SET_FREQUENCY,                      /**< Updates the frequency of a PWM channel */
    PWM_ACTION_END,

    NUM_OPTIONS,
    INVALID
  };

  enum class Switchable : uint8_t
  {
    NUM_OPTIONS,
    INVALID
  };

  enum class SwitchableState : uint8_t
  {
    ON,
    OFF,
    NUM_OPTIONS,
    INVALID
  };

  enum class DriverData : size_t
  {
    IS_CONFIGURED,
    DRIVER_CONFIG,
    CHANNEL_CONFIG,
    NUM_OPTIONS,
    INVALID
  };

  enum class Direction : uint8_t
  {
    COUNT_UP, /**< Counts up from min, overflows, then starts counting up again */
    COUNT_DN, /**< Counts down from max, underflows, then starts counting down again */

    NUM_OPTIONS
  };

  enum class Function : uint8_t
  {
    INVALID,
    INPUT_CAPTURE,
    OUTPUT_COMPARE,
    PWM_OUTPUT,
    ONE_PULSE_OUTPUT,
    ENCODER,

    NUM_OPTIONS
  };

  enum class Event : uint8_t
  {
    INVALID,
    // Fill in when needed
    NUM_OPTIONS
  };

  enum class Peripheral : uint8_t
  {
    TIMER1,
    TIMER2,
    TIMER3,
    TIMER4,
    TIMER5,
    TIMER6,
    TIMER7,
    TIMER8,
    TIMER9,
    TIMER10,
    TIMER11,
    TIMER12,
    TIMER13,
    TIMER14,
    TIMER15,
    TIMER16,
    LPTIMER1,
    LPTIMER2,

    NUM_OPTIONS,
    NOT_SUPPORTED
  };

  enum class Channel : uint8_t
  {
    CHANNEL_1,
    CHANNEL_2,
    CHANNEL_3,
    CHANNEL_4,
    CHANNEL_5,
    CHANNEL_6,

    NUM_OPTIONS,
    INVALID
  };


  /*-------------------------------------------------------------------------------
  Namespaces
  -------------------------------------------------------------------------------*/
  namespace Encoder
  {
    struct Config
    {
      // Currently not used
    };
  }  // namespace Encoder


  namespace InputCapture
  {
    struct Config
    {
      // Currently not used
    };
  }  // namespace InputCapture


  namespace OnePulse
  {
    struct Config
    {
      // Currently not used
    };
  }  // namespace OnePulse


  namespace OutputCompare
  {
    struct Config
    {
      // Currently not used
    };
  }  // namespace OutputCompare


  namespace PWM
  {
    enum class Polarity
    {
      ACTIVE_HIGH,
      ACTIVE_LOW,

      NUM_OPTIONS
    };

    enum class Mode
    {
      EDGE_ALIGNED,
      CENTER_ALIGNED,
      ASYMMETRIC,
      COMBINED,

      NUM_OPTIONS,
    };

    struct Config
    {
      Chimera::Timer::Peripheral peripheral; /**< Timer peripheral in use */
      Chimera::Timer::Channel outputChannel; /**< Channel to configure the PWM output on */
      Mode mode;                             /**< The PWM mode to operate as */
      size_t compareMatch;                   /**< Value to compare/match to that generates an event */
      Polarity polarity;                     /**< Idle state when signal not asserted */
      size_t frequency;                      /**< Desired frequency of the PWM output */
      uint8_t dutyCycle;                     /**< Desired duty cycle of the PWM output */
      bool validity;
    };
  }  // namespace PWM


  /*-------------------------------------------------------------------------------
  Structures
  -------------------------------------------------------------------------------*/

  struct DriverConfig
  {
    bool validity;  /**< Decides if the configuration settings are valid */
    bool overwrite; /**< Allows the config to update the entire timer peripheral (multiple channels share one peripheral) */
    Peripheral peripheral;    /**< Which peripheral to configure */
    Direction countDirection; /**< Which direction the free-running counter should count */
    size_t reloadValue;       /**< Value to load when the counter overflows */
    size_t prescaler;         /**< Divides the peripheral source clock to provide the tick clock */
  };

  union CoreFeatureInit
  {
    DriverConfig base;
    Encoder::Config encoder;
    InputCapture::Config inputCapture;
    OnePulse::Config onePulse;
    OutputCompare::Config outputCompare;
    PWM::Config pwm;
  };


  /*-------------------------------------------------
  Structures associated with DriverData
  -------------------------------------------------*/
  struct DataRequest_ChannelConfig_t
  {
    Channel channel;         /**< Which channel to request the data for */
    CoreFeatureInit cfgData; /**< Memory to place config data into */
    bool validity;           /**< Whether or not the data is valid */
  };

  struct DataRequest_DriverConfig_t
  {
    CoreFeatureInit cfgData; /**< Memory to place config data into */
    bool validity;           /**< Whether or not the data is valid */
  };


  /*-------------------------------------------------
  Structures associated with DriverAction
  -------------------------------------------------*/
  struct DriverAction_PWMDutyCycle_t
  {
    Channel channel;  /**< The channel to update */
    size_t dutyCycle; /**< The new duty cycle to be set */
  };


  struct Descriptor
  {
  };

  namespace Backend
  {
    struct DriverRegistration
    {
      bool isSupported;
      Chimera::Status_t ( *initialize )( void );
      Chimera::Status_t ( *reset )( void );
      size_t ( *millis )( void );
      size_t ( *micros )( void );
      void ( *delayMilliseconds )( const size_t );
      void ( *delayMicroseconds )( const size_t );
      ITimer_sPtr ( *createSharedInstance )( const Chimera::Timer::Peripheral );
      ITimer_uPtr ( *createUniqueInstance )( const Chimera::Timer::Peripheral );
      ITimer_rPtr ( *createUnsafeInstance )( const Chimera::Timer::Peripheral );
    };
  }  // namespace Backend
}  // namespace Chimera::Timer

#endif /* !CHIMERA_TIMER_TYPES_HPP */
