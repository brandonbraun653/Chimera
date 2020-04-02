/********************************************************************************
 *  File Name:
 *    system_intf.hpp
 *
 *  Description:
 *    Models the Chimera system interface
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SYSTEM_INTERFACE_HPP
#define CHIMERA_SYSTEM_INTERFACE_HPP

/* STL Includes */
#include <cstdint>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/src/system/system_types.hpp>

namespace Chimera::System
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
}  // namespace Chimera::System

#endif /* !CHIMERA_SYSTEM_INTERFACE_HPP */
