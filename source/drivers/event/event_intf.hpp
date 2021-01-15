/********************************************************************************
 *   File Name:
 *    event_intf.hpp
 *
 *   Description:
 *    Models the Chimera Event interface
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_EVENT_INTERFACE_HPP
#define CHIMERA_EVENT_INTERFACE_HPP

/* STL Includes */
#include <cstdint>
#include <functional>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/source/drivers/event/event_types.hpp>
#include <Chimera/source/drivers/peripherals/interrupt/interrupt_types.hpp>

namespace Chimera::Event
{
  /**
   *  Interface describing a methodology for asynchronously notifying other sections
   *  of code that some kind of event has occured. Most of the thread safety is left
   *  up to the back end driver, should it be needed.
   */
  class ListenerInterface
  {
  public:
    virtual ~ListenerInterface() = default;

    /**
     *  Attach a listener to get notified when an event occurs.
     *
     *  @param[in]  listener            The event to be notified on
     *  @param[in]  timeout             How long to wait for access (mS)
     *  @param[out] registrationID      The notification variable
     *  @return Chimera::Status_t
     *
     *  |   Return Value   |                       Explanation                      |
     *  |:----------------:|:------------------------------------------------------:|
     *  |               OK | The listener was attached                              |
     *  |             FAIL | The listener could not be attached                     |
     *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function          |
     *  |          TIMEOUT | The system timed out while accessing resources         |
     *  |    NOT_SUPPORTED | This function is not supported by the low level driver |
     */
    virtual Chimera::Status_t registerListener( Chimera::Event::Actionable &listener, const size_t timeout,
                                                size_t &registrationID ) = 0;

    /**
     *   Remove an event listener
     *
     *   @param[in]  registrationID     The id returned when registering the listener
     *   @param[in]  timeout            How long to wait for access (mS)
     *   @return Chimera::Status_t
     *
     *  |   Return Value   |                       Explanation                      |
     *  |:----------------:|:------------------------------------------------------:|
     *  |               OK | The notifier(s) was removed                            |
     *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function          |
     *  |          TIMEOUT | The system timed out while accessing resources         |
     *  |    NOT_SUPPORTED | This function is not supported by the low level driver |
     */
    virtual Chimera::Status_t removeListener( const size_t registrationID, const size_t timeout ) = 0;
  };

}  // namespace Chimera::Event

#endif /* !CHIMERA_EVENT_INTERFACE_HPP */
