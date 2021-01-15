/********************************************************************************
 *  File Name:
 *    interrupt_intf.hpp
 *
 *  Description:
 *    Interface to the interrupt layer
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_INTERRUPT_INTERFACE_HPP
#define CHIMERA_INTERRUPT_INTERFACE_HPP

/* Chimera Includes */
#include <Chimera/common>

namespace Chimera::Interrupt
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
}  // namespace Chimera::Interrupt

#endif  /* !CHIMERA_INTERRUPT_INTERFACE_HPP */
