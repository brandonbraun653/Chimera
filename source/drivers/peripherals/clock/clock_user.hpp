/********************************************************************************
 *  File Name:
 *    clock.hpp
 *
 *  Description:
 *    High level clock driver for Chimera
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_CLOCK_HPP
#define CHIMERA_CLOCK_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/source/drivers/peripherals/clock/clock_types.hpp>

namespace Chimera::Clock
{
  Chimera::Status_t initialize();
  Chimera::Status_t periphEnable( const Chimera::Peripheral::Type periph );
  Chimera::Status_t periphDisable( const Chimera::Peripheral::Type periph );
  Chimera::Status_t enableClock( const Chimera::Clock::Bus bus );
  Chimera::Status_t disableClock( const Chimera::Clock::Bus bus );
  bool isEnabled( const Chimera::Clock::Bus bus );
  size_t getFrequency( const Chimera::Clock::Bus bus );
  Chimera::Status_t setFrequency( const Chimera::Clock::Bus bus, const size_t freq );
}  // namespace Chimera::Clock

#endif /* !CHIMERA_CLOCK_HPP */
