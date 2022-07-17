/********************************************************************************
 *  File Name:
 *    timer_types.hpp
 *
 *	 Description:
 *    Types for the Chimera Timer module
 *
 *  2020-2022 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_TIMER_TYPES_HPP
#define CHIMERA_TIMER_TYPES_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <cstdlib>
#include <memory>
#include <Chimera/common>

namespace Chimera::Timer
{
  /*---------------------------------------------------------------------------
  Forward Declarations
  ---------------------------------------------------------------------------*/
  class ITimer;

  /*---------------------------------------------------------------------------
  Enumerations
  ---------------------------------------------------------------------------*/
  /**
   * @brief Lists the supported class interfaces available to a timer resource
   */
  enum class TimerInterface : uint8_t
  {
    SOFTWARE_EVENT,
    HARDWARE_TRIGGER,
    PWM_GENERATION,

    NUM_OPTIONS,
    INVALID
  };

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

  enum class Instance : uint8_t
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

  enum class Output : uint8_t
  {
    OUTPUT_1P,  /* Positive logic output */
    OUTPUT_1N,  /* Negative (complementary) output */
    OUTPUT_2P,
    OUTPUT_2N,
    OUTPUT_3P,
    OUTPUT_3N,
    OUTPUT_4P,
    OUTPUT_4N,
    OUTPUT_5P,
    OUTPUT_5N,
    OUTPUT_6P,
    OUTPUT_6N,

    NUM_OPTIONS,
    INVALID
  };

  namespace Backend
  {
    struct DriverConfig
    {
      bool isSupported;

      /**
       *  Initializes the timer module
       *
       *  @return Chimera::Status_t
       */
      Chimera::Status_t ( *initialize )( void );

      /**
       *  Resets the timer module
       */
      Chimera::Status_t ( *reset )( void );

      /**
       *  Returns the number of milliseconds elapsed since the beginning
       *  of program execution.
       *
       *  @return size_t
       */
      size_t ( *millis )( void );

      /**
       *  Returns the number of microseconds elapsed since the beginning
       *  of program execution.
       *
       *  @return size_t
       */
      size_t ( *micros )( void );

      /**
       *  Delays the current thread of execution a given number of milliseconds.
       *  If a scheduler is present, the delay will be non-blocking to other threads,
       *  otherwise the system will halt the specified amount.
       *
       *  @param[in]  val   The number of milliseconds to delay
       *  @return void
       */
      void ( *delayMilliseconds )( const size_t );

      /**
       *  Delays the current thread of execution a given number of microseconds.
       *  If a scheduler is present, the delay will be non-blocking to other threads,
       *  otherwise the system will halt the specified amount.
       *
       *  @note Depending on the tick rate of the scheduler, non-blocking microsecond
       *        delays may not be possible.
       *
       *  @param[in]  val   The number of microseconds to delay
       *  @return void
       */
      void ( *delayMicroseconds )( const size_t );

      /**
       *  Busy-wait delay for a number of milliseconds
       *
       *  @param[in]  val     Milliseconds to delay
       *  @return void
       */
      void ( *blockDelayMilliseconds )( const size_t );

      /**
       *  Busy-wait delay for a number of microseconds.
       *
       *  @param[in]  val     Microseconds to delay
       *  @return void
       */
      void ( *blockDelayMicroseconds )( const size_t );
    };
  }  // namespace Backend
}  // namespace Chimera::Timer

#endif /* !CHIMERA_TIMER_TYPES_HPP */
