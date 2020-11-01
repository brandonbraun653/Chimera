/********************************************************************************
 *  File Name:
 *    exti_user.hpp
 *
 *  Description:
 *    User interface to Chimera External Interrupt driver
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_EXTI_USER_INTERFACE_HPP
#define CHIMERA_EXTI_USER_INTERFACE_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/function>
#include <Chimera/source/drivers/peripherals/exti/exti_types.hpp>

namespace Chimera::EXTI
{
  /**
   *  Initializes the external interrupt hardware driver and its resources
   *  @return Chimera::Status_t
   */
  Chimera::Status_t open();

  /**
   *  Tears down the external interrupt hardware driver and its resources
   *  @return Chimera::Status_t
   */
  Chimera::Status_t close();

  /**
   *  Configures the interrupt line
   *
   *  @param[in]  listener      The listener being configured
   *  @param[in]  callback      What callback to invoke upon interrupt firing
   *  @return Chimera::Status_t
   */
  Chimera::Status_t attach( const EventLine_t listener, Chimera::Function::vGeneric callback );

  /**
   *  Disables the given interrupt and resets it to defaults
   *
   *  @param[in]  listener      Which interrupt listener to detach from
   *  @return Chimera::Status_t
   */
  Chimera::Status_t detach( const EventLine_t listener );

  /**
   *  Manually trigger an interrupt even on the given line
   *
   *  @param[in]  listener      Which interrupt listener is being triggered
   *  @return Chimera::Status_t
   */
  Chimera::Status_t trigger( const EventLine_t listener );

  /**
   *  Places the interrupt line in a disabled state so it cannot fire.
   *
   *  @param[in]  listener      Which interrupt listener is being disabled
   *  @return Chimera::Status_t
   */
  Chimera::Status_t disable( const EventLine_t listener );

  /**
   *  Enables the interrupt so that it can listen for events
   *
   *  @param[in]  listener      Which interrupt listener is being enabled
   *  @return Chimera::Status_t
   */
  Chimera::Status_t enable( const EventLine_t listener );

  /**
   *  How many external interrupt lines are available on the system
   *  @return EventLine_t
   */
  EventLine_t numInterruptLines();

}  // namespace Chimera::EXTI

#endif  /* !CHIMERA_EXTI_USER_INTERFACE_HPP */
