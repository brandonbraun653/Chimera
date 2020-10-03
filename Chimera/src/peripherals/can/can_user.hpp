/********************************************************************************
 *  File Name:
 *    can.hpp
 *
 *  Description:
 *    Implements an interface to create a Chimera CAN peripheral
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_CAN_HPP
#define CHIMERA_CAN_HPP

/* STL Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/src/peripherals/can/can_intf.hpp>
#include <Chimera/src/peripherals/can/can_types.hpp>

namespace Chimera::CAN
{
  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  Chimera::Status_t initialize();
  Chimera::Status_t reset();
  Driver_sPtr getDriver( const Channel channel );
}  // namespace Chimera::CAN

#endif  /* !CHIMERA_CAN_HPP */
