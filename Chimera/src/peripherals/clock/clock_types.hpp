/********************************************************************************
 *  File Name:
 *    clock_types.hpp
 *
 *  Description:
 *    Types used in describing on board clocks
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_CLOCK_TYPES_HPP
#define CHIMERA_CLOCK_TYPES_HPP


/* Chimera Includes */
#include <Chimera/common>

namespace Chimera::Clock
{
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
    PLLCLK,
    HCLK,
    PCLK1,
    APB1 = PCLK1,
    PCLK2,
    APB2 = PCLK2,

    MAX_NUM_CLOCKS
  };


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
      Chimera::Status_t ( *initialize )();
      Chimera::Status_t ( *periphEnable )( const Chimera::Peripheral::Type periph );
      Chimera::Status_t ( *periphDisable )( const Chimera::Peripheral::Type periph );
      Chimera::Status_t ( *enableClock )( const Chimera::Clock::Bus bus );
      Chimera::Status_t ( *disableClock )( const Chimera::Clock::Bus bus );
      Chimera::Status_t ( *setFrequency )( const Chimera::Clock::Bus bus, const size_t freq );
      bool ( *isEnabled )( const Chimera::Clock::Bus bus );
      size_t ( *getFrequency )( const Chimera::Clock::Bus bus );
    };
  }  // namespace Backend
}  // namespace Chimera::Clock

#endif /* !CHIMERA_CLOCK_TYPES_HPP */
