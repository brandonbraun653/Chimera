/******************************************************************************
 *  File Name:
 *    timer_intf.hpp
 *
 *	Description:
 *    Chimera timer interface common to all peripherals
 *
 *  2020-2022 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_TIMER_INTERFACE_HPP
#define CHIMERA_TIMER_INTERFACE_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/event>
#include <Chimera/thread>
#include <Chimera/source/drivers/peripherals/timer/timer_common_types.hpp>

namespace Chimera::Timer
{
  /*---------------------------------------------------------------------------
  Public Functions
  ---------------------------------------------------------------------------*/
  namespace Backend
  {
    /**
     *  Registers the backend driver with Chimera
     *
     *  @param[in]  registry    Chimera's copy of the driver interface
     *  @return Chimera::Status_t
     */
    extern Chimera::Status_t registerDriver( DriverConfig &registry );
  }  // namespace Backend

}  // namespace Chimera::Timer

#endif /* !CHIMERA_TIMER_INTERFACE_HPP */
