/********************************************************************************
 *   File Name:
 *    serial_types.hpp
 *
 *   Description:
 *    Chimera Serial types
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_EVENT_TYPES_HPP
#define CHIMERA_EVENT_TYPES_HPP

/* C++ Includes */
#include <cstdint>
#include <memory>

/* FreeRTOS Includes */
#ifdef __cplusplus
extern "C"
{
#include "FreeRTOS.h"
#include "event_groups.h"
}
#endif /* __cplusplus */

namespace Chimera::Event
{
  class Notifier;
  using Notifier_sPtr = std::shared_ptr<Notifier>;
  using Notifier_uPtr = std::unique_ptr<Notifier>;
  

  enum class Trigger : size_t
  {
    READ_COMPLETE = 0,  /**< A read was completed */
    WRITE_COMPLETE,     /**< A write was completed */
    ERROR,              /**< Catch all error case */
  };

  /**
   *  FreeRTOS specific event flags used by hardware drivers
   *  to signal that a particular event has occured.
   */
  enum Flags : EventBits_t
  {
    BIT_READ_COMPLETE  = ( 1u << 0 ),
    BIT_WRITE_COMPLETE = ( 1u << 1 ),

#if configUSE_16_BIT_TICKS == 0

#endif
  };

}  // namespace Chimera::Event

#endif /* !CHIMERA_EVENT_TYPES_HPP */