/********************************************************************************
 *  File Name:
 *    event.hpp
 *
 *  Description:
 *    Implements a common interface for events
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_EVENT_HPP
#define CHIMERA_EVENT_HPP

/* STL Includes */
#include <vector>

/* Chimera Includes */
#include <Chimera/source/drivers/event/event_types.hpp>

namespace Chimera::Event
{
  /**
   *  Notifies a listener that some event has occurred. If a function
   *  has been registered with the listener, it will be invoked.
   *
   *  @param[in]  event       The event that occurred
   *  @param[in]  listener    The listener to be notified of the event
   *  @param[in]  value       Optional value to pass in to the listener object
   *  @return bool            True if the listener successfully was notified, else false
   */
  bool notifyListener( const Trigger event, Actionable &listener, uint32_t value );

  /**
   *  Provides the same functionality as notifyListener(), but iterates over a list.
   *
   *  @param[in]  event       The event that occurred
   *  @param[in]  list        The list of listeners to be notified of the event
   *  @param[in]  value       Optional value to pass in to the listener object
   *  @return bool            True if all listeners were successfully notified, else false
   */
  bool notifyListenerList( const Trigger event, ActionableList &list, uint32_t value );

}  // namespace Chimera::Event

#endif /* !CHIMERA_EVENT_HPP */
