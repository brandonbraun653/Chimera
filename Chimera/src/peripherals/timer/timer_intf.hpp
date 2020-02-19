/********************************************************************************
 *  File Name:
 *    timer_intf.hpp
 *
 *	 Description:
 *    Chimera timer interface
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_TIMER_INTERFACE_HPP
#define CHIMERA_TIMER_INTERFACE_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/src/peripherals/timer/timer_types.hpp>

namespace Chimera::Timer
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
}

#endif  /* !CHIMERA_TIMER_INTERFACE_HPP */
