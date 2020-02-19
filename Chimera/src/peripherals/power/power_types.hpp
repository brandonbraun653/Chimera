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
#ifndef CHIMERA_POWER_TYPES_HPP
#define CHIMERA_POWER_TYPES_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/src/peripherals/peripheral_types.hpp>

namespace Chimera::Power
{
  enum class State : uint8_t
  {
    OFF,
    ON,
    ON_LOW_POWER,
    INVALID
  };

  namespace Backend
  {
    using Initialize_FPtr    = Chimera::Status_t ( * )( void );
    using Reset_FPtr         = Chimera::Status_t ( * )( void );
    using PeriphEnable_FPtr  = Chimera::Status_t ( * )( const Chimera::Peripheral::Type );
    using PeriphDisable_FPtr = Chimera::Status_t ( * )( const Chimera::Peripheral::Type );
    using SetPowerState_FPtr = Chimera::Status_t ( * )( const Chimera::Power::State );
    using GetPowerState_FPtr = Chimera::Power::State ( * )( const Chimera::Peripheral::Type );

    struct DriverConfig
    {
      bool isSupported;
      Initialize_FPtr initialize;
      Reset_FPtr reset;
      PeriphEnable_FPtr periphEnable;
      PeriphDisable_FPtr periphDisable;
      SetPowerState_FPtr setPowerState;
      GetPowerState_FPtr getPowerState;
    };
  }  // namespace Backend
}  // namespace Chimera::Power

#endif /* !CHIMERA_POWER_TYPES_HPP */