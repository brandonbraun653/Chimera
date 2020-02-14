/********************************************************************************
 *   File Name:
 *    power_intf.hpp
 *
 *   Description:
 *    Models the Chimera power interface
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_POWER_INTERFACE_HPP
#define CHIMERA_POWER_INTERFACE_HPP

/* Chimera Includes */
#include <Chimera/types/common_types.hpp>
#include <Chimera/types/peripheral_types.hpp>
#include <Chimera/types/power_types_fwd.hpp>


namespace Chimera::Power
{
  class InfoInterface
  {
  public:
    virtual ~InfoInterface() = default;

    virtual float systemVCC() = 0;
  };

  class InfoInterfaceDisabled : InfoInterface
  {
  public:
    float systemVCC() final override
    {
      return 0.0f;
    }
  };

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

#endif /* !CHIMERA_POWER_INTERFACE_HPP */