/********************************************************************************
 *  File Name:
 *    interrupt_user.hpp
 *
 *  Description:
 *    User interface to the interrupt peripheral
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_INTERRUPT_USER_HPP
#define CHIMERA_INTERRUPT_USER_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/source/drivers/peripherals/interrupt/interrupt_types.hpp>

namespace Chimera::Interrupt
{
  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  Chimera::Status_t initialize();
  Chimera::Status_t reset();
  Chimera::Status_t registerISRHandler( const Peripheral::Type type, const Signal_t signal, const SignalCallback &callback );
  Chimera::Status_t disableISR( const Signal_t signal );
  Chimera::Status_t enableISR( const Signal_t signal );
}  // namespace Chimera::Interrupt

#endif /* !CHIMERA_INTERRUPT_USER_HPP */
