/********************************************************************************
 *  File Name:
 *    power.hpp
 *
 *  Description:
 *    Models the system power interface
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_POWER_HPP
#define CHIMERA_POWER_HPP

/* STL Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/src/peripherals/peripheral_types.hpp>
#include <Chimera/src/peripherals/power/power_intf.hpp>
#include <Chimera/src/peripherals/power/power_types.hpp>

namespace Chimera::Power
{
  Chimera::Status_t initialize();

  Chimera::Status_t reset();
  
  /**
   *  Turns on the power for the given peripheral.
   *
   *  @note Must be implemented by the backend driver as it is processor specific
   *
   *  @param[in]  periph    The peripheral to turn on the power for
   *  @return Chimera::Status_t
   *
   *  |  Return Value |             Explanation            |
   *  |:-------------:|:----------------------------------:|
   *  |            OK | The power was successfully enabled |
   *  |          FAIL | The power could not be enabled     |
   *  | NOT_SUPPORTED | This peripheral is not supported   |
   */
  Chimera::Status_t periphEnable( const Chimera::Peripheral::Type periph );

  /**
   *  Turns off the power for the given peripheral.
   *
   *  @note Must be implemented by the backend driver as it is processor specific
   *
   *  @param[in]  periph    The peripheral to turn off the power for
   *  @return Chimera::Status_t
   *
   *  |  Return Value |             Explanation             |
   *  |:-------------:|:-----------------------------------:|
   *  |            OK | The power was successfully disabled |
   *  |          FAIL | The power could not be disabled     |
   *  | NOT_SUPPORTED | This peripheral is not supported    |
   */
  Chimera::Status_t periphDisable( const Chimera::Peripheral::Type periph );

  /**
   *  Configures a power state for the processor or peripheral. The scope
   *  of this function is really only limited by the values defined by
   *  the backend driver for Chimera::Power::State. It could be as coarse
   *  or as fine grained as the developer likes.
   *
   *  @param[in]  state       The given state to transition into
   *  @return Chimera::Status_t
   *
   *  |  Return Value |                Explanation               |
   *  |:-------------:|:----------------------------------------:|
   *  |            OK | The power state was successfully updated |
   *  |          FAIL | The power state failed to be updated     |
   *  | NOT_SUPPORTED | The power state is not supported         |
   */
  Chimera::Status_t setPowerState( const Chimera::Power::State state );

  /**
   *  Retreives the current power status of the requested peripheral
   *
   *  @param[in]  periph      The peripheral to be queried
   *  @return Chimera::Power::State
   */
  Chimera::Power::State getPowerState( const Chimera::Peripheral::Type periph );

}  // namespace Chimera::Power

#endif /* !CHIMERA_POWER_HPP */