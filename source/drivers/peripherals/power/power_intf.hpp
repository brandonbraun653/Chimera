/********************************************************************************
 *  File Name:
 *    power_intf.hpp
 *
 *  Description:
 *    Models the Chimera power interface
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_POWER_INTERFACE_HPP
#define CHIMERA_POWER_INTERFACE_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/source/drivers/peripherals/power/power_types.hpp>

namespace Chimera::Power
{
  namespace Backend
  {
    /**
     *  Registers the backend driver with Chimera
     *
     *  @param[in]  registry    Chimera's copy of the driver interface
     *  @return Chimera::Status_t
     */
    extern Chimera::Status_t registerDriver( DriverConfig &registry );
  }

  class InfoInterface
  {
  public:
    virtual ~InfoInterface() = default;

    virtual float systemVCC() = 0;
  };

}  // namespace Chimera::Power

#endif /* !CHIMERA_POWER_INTERFACE_HPP */