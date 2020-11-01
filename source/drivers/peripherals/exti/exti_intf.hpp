/********************************************************************************
 *  File Name:
 *    exti_intf.hpp
 *
 *  Description:
 *    Implementer interface for the External Interrupt driver
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_EXTI_INTERFACE_HPP
#define CHIMERA_EXTI_INTERFACE_HPP

/* STL Includes */
#include <cstddef>

/* Chimera Includes */
#include <Chimera/source/drivers/peripherals/exti/exti_types.hpp>


namespace Chimera::EXTI
{
  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
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
}  // namespace Chimera::EXTI

#endif /* !CHIMERA_EXTI_INTERFACE_HPP */
