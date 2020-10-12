/********************************************************************************
 *  File Name:
 *    event_types.hpp
 *
 *  Description:
 *    Chimera Event types
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_EVENT_TYPES_HPP
#define CHIMERA_EVENT_TYPES_HPP

/* STL Includes */
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <memory>
#include <variant>
#include <vector>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/callback>
#include <Chimera/src/threading/semaphore.hpp>

namespace Chimera::Event
{
  enum class Trigger : size_t
  {
    TRIGGER_INVALID,           /**< Special case used for initialization */
    TRIGGER_READ_COMPLETE,     /**< A read was completed */
    TRIGGER_WRITE_COMPLETE,    /**< A write was completed */
    TRIGGER_TRANSFER_COMPLETE, /**< A transfer of some sort completed (bi-directional) */
    TRIGGER_SYSTEM_ERROR,      /**< Catch all error case */
    TRIGGER_DATA_AVAILABLE,    /**< Some kind of data is ready for processing */

    NUM_OPTIONS,
    UNKNOWN
  };

  enum class ListenerType : size_t
  {
    LISTENER_INVALID,      /**< Listener is invalid */
    LISTENER_ATOMIC,       /**< Listener is an atomically writeable variable */
    LISTENER_SEMAPHORE,    /**< Listener is a semaphore to be released/given to */
    LISTENER_FUNCTION,     /**< Listener is a callback function to be invoked */
    LISTENER_ISR_CALLBACK, /**< Special listener that executes inside an ISR */

    NUM_OPTIONS,
    UNKNOWN
  };

  union ListenerObject
  {
    uint32_t *atomic_listener;
    Chimera::Callback::EventFunction event_callback_listener;
    Chimera::Callback::ISRFunction isr_callback_listener;
    Chimera::Threading::BinarySemaphore *semaphore_listener;
  };

  struct Actionable
  {
    size_t id;             /**< Uniquely identifies this Actionable */
    Trigger trigger;       /**< Which kind of event triggers this Actionable */
    ListenerType type;     /**< What kind of Actionable is stored in the handle */
    ListenerObject object; /**< The listener object to be invoked */
  };

  using ActionableList = std::vector<Actionable>;

}  // namespace Chimera::Event

#endif /* !CHIMERA_EVENT_TYPES_HPP */
