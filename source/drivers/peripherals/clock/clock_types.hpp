/******************************************************************************
 *  File Name:
 *    clock_types.hpp
 *
 *  Description:
 *    Types used in describing on board clocks
 *
 *  2020-2021 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_CLOCK_TYPES_HPP
#define CHIMERA_CLOCK_TYPES_HPP

/* Chimera Includes */
#include <Chimera/common>

namespace Chimera::Clock
{
  /*---------------------------------------------------------------------------
  Enumerations
  ---------------------------------------------------------------------------*/
  enum class Bus
  {
    UNKNOWN_BUS,

    /* STM32 Common Bus Types */
    HSE,
    LSE,
    LSI,
    SYSCLK,
    MSI,
    HSI16,
    RC48,
    PLLP,
    PLLQ,
    PLLR,
    HCLK,
    AHB = HCLK,
    PCLK1,
    APB1 = PCLK1,
    PCLK2,
    APB2 = PCLK2,

    MAX_NUM_CLOCKS
  };

  /*---------------------------------------------------------------------------
  Structures
  ---------------------------------------------------------------------------*/
  namespace Backend
  {
    /**
     *  Backend driver configuration function pointers. For documentation on
     *  what each function does and its input/output arguments, see the main
     *  header file for this module.
     */
    struct DriverConfig
    {
      bool isSupported;

      /**
       *  Initialize the Clock driver resources. Must be called before
       *  any other functionality is used.
       *
       *  @return Chimera::Status_t
       */
      Chimera::Status_t ( *initialize )();

      /**
       *  Turns on the clock to the given peripheral. The actual clock
       *  source must be configured before this function is called.
       *
       *  @param[in]  periph    The peripheral to turn on the clock for
       *  @return Chimera::Status_t
       *
       *  |  Return Value |             Explanation            |
       *  |:-------------:|:----------------------------------:|
       *  |            OK | The clock was successfully enabled |
       *  |          FAIL | The clock could not be enabled     |
       *  | NOT_SUPPORTED | This peripheral is not supported   |
       */
      Chimera::Status_t ( *periphEnable )( const Chimera::Peripheral::Type periph );

      /**
       *  Turns off the clock to the given peripheral.
       *
       *  @param[in]  periph    The peripheral to turn off the clock for
       *  @return Chimera::Status_t
       *
       *  |  Return Value |             Explanation             |
       *  |:-------------:|:-----------------------------------:|
       *  |            OK | The clock was successfully disabled |
       *  |          FAIL | The clock could not be disabled     |
       *  | NOT_SUPPORTED | This peripheral is not supported    |
       */
      Chimera::Status_t ( *periphDisable )( const Chimera::Peripheral::Type periph );

      /**
       *  Enables the given clock bus
       *
       *  @param[in]  bus       The clock bus to be turned on
       *  @return Chimera::Status_t
       *
       *  |  Return Value |             Explanation            |
       *  |:-------------:|:----------------------------------:|
       *  |            OK | The clock was successfully enabled |
       *  |          FAIL | The clock could not be enabled     |
       *  | NOT_SUPPORTED | This clock is not supported        |
       */
      Chimera::Status_t ( *enableClock )( const Chimera::Clock::Bus bus );

      /**
       *  Disables the given clock bus
       *
       *  @param[in]  bus       The clock bus to be turned off
       *  @return Chimera::Status_t
       *
       *  |  Return Value |             Explanation             |
       *  |:-------------:|:-----------------------------------:|
       *  |            OK | The clock was successfully disabled |
       *  |          FAIL | The clock could not be disabled     |
       *  | NOT_SUPPORTED | This clock is not supported         |
       */
      Chimera::Status_t ( *disableClock )( const Chimera::Clock::Bus bus );

      /**
       *  Sets the operating frequency of a clock bus in Hz.
       *
       *  @note If the specified clock is an external source, it will be cached
       *        for later lookup.
       *
       *  @param[in]  bus       The clock bus to update the frequency on
       *  @param[in]  freq      The desired frequency in Hz
       *  @return Chimera::Status_t
       *
       *  |  Return Value |             Explanation            |
       *  |:-------------:|:----------------------------------:|
       *  |            OK | The clock was successfully updated |
       *  |          FAIL | The clock failed to be updated     |
       *  | NOT_SUPPORTED | The clock is not supported         |
       */
      Chimera::Status_t ( *setFrequency )( const Chimera::Clock::Bus bus, const size_t freq );

      /**
       *  Checks if the given bus has its clock enabled
       *
       *  @param[in]  bus       The clock bus to check
       *  @return bool
       */
      bool ( *isEnabled )( const Chimera::Clock::Bus bus );

      /**
       *  Gets the current operating frequency of the clock in Hz
       *
       *  @param[in]  bus       The clock bus to check
       *  @return size_t
       */
      size_t ( *getFrequency )( const Chimera::Clock::Bus bus );
    };
  }  // namespace Backend
}  // namespace Chimera::Clock

#endif /* !CHIMERA_CLOCK_TYPES_HPP */
