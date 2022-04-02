/******************************************************************************
 *  File Name:
 *    timer_trigger_intf.hpp
 *
 *  Description:
 *    Timer view for acting as a trigger to other peripherals
 *
 *  2022 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_TIMER_PERIPHERAL_TRIGGER_INTF_HPP
#define CHIMERA_TIMER_PERIPHERAL_TRIGGER_INTF_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/source/drivers/peripherals/timer/timer_common_types.hpp>
#include <Chimera/source/drivers/peripherals/timer/timer_intf.hpp>


namespace Chimera::Timer
{
  /*-----------------------------------------------------------------------------
  Enumerations
  -----------------------------------------------------------------------------*/
  enum TriggerOutput : uint8_t
  {
    TRIG_OUT_1,
    TRIG_OUT_2,
    TRIG_OUT_3,
    TRIG_OUT_4,
    TRIG_OUT_5,
    TRIG_OUT_6,

    TRIG_OUT_NUM_OPTIONS,
    TRIG_OUT_INVALID
  };


  class HardwareTrigger // Really just a time base interface with an option to control the HW output line
  {
  public:


  private:
    void *mDriver; /**< Opaque pointer to the implementer's driver */
  };
}  // namespace Chimera::Timer

#endif  /* !CHIMERA_TIMER_PERIPHERAL_TRIGGER_INTF_HPP */
