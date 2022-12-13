/******************************************************************************
 *  File Name:
 *    power.hpp
 *
 *  Description:
 *    Models the system power interface
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_POWER_HPP
#define CHIMERA_POWER_HPP

/* STL Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/source/drivers/peripherals/peripheral_types.hpp>
#include <Chimera/source/drivers/peripherals/power/power_intf.hpp>
#include <Chimera/source/drivers/peripherals/power/power_types.hpp>

namespace Chimera::Power
{
  Chimera::Status_t initialize();
  Chimera::Status_t reset();
  Chimera::Status_t periphEnable( const Chimera::Peripheral::Type periph );
  Chimera::Status_t periphDisable( const Chimera::Peripheral::Type periph );
  Chimera::Status_t setPowerState( const Chimera::Power::State state );
  Chimera::Power::State getPowerState( const Chimera::Peripheral::Type periph );
}  // namespace Chimera::Power

#endif /* !CHIMERA_POWER_HPP */
