/******************************************************************************
 *  File Name:
 *    clock_intf.hpp
 *
 *  Description:
 *    Provides the interace with which to interact with a clock driver
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_CLOCK_INTERFACE_HPP
#define CHIMERA_CLOCK_INTERFACE_HPP

/* STL Includes */
#include <cstdint>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/source/drivers/peripherals/clock/clock_types.hpp>

namespace Chimera::Clock::Backend
{
  /**
   *  Registers the backend driver with Chimera
   *
   *  @param[in]  registry    Chimera's copy of the driver interface
   *  @return Chimera::Status_t
   */
  extern Chimera::Status_t registerDriver( DriverConfig &registry );
}  // namespace Chimera::Clock::Backend

#endif /* !CHIMERA_CLOCK_INTERFACE_HPP */