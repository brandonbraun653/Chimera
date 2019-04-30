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

/* C++ Includes */
#include <cstdint>
#include <functional>

/* Chimera Includes */
#include <Chimera/preprocessor.hpp>
#include <Chimera/types/common_types.hpp>
#include <Chimera/types/event_types.hpp>

#if defined( USING_FREERTOS )
#include <Chimera/threading.hpp>
#endif

namespace Chimera::Event
{
  /**
   *  Interface describing a methodology for asynchronously notifying other sections
   *  of code that some kind of event has occured. Most of the thread safety is left
   *  up to the back end driver, should it be needed. 
   */
  class Notifier
  {
  public:
    virtual ~Notifier() = default;
    
    /**
     *  Attach a signal to get notified when an event occurs. The notifier is 
     *  intentionally volatile 8-bit data as the read/write access is atomic on 
     *  most MCUs these days. This is a lightweight alternative to std::atomic.
     *  
     *  @note Queueing multiple notifiers for an event is optional
     *  @note The actual value written to the notifier upon event occurance is left up to the back end driver
     *
     *  @param[in]  event         The event to be notified on
     *  @param[in]  notifier      The notification variable
     *  @return Chimera::Status_t
     *
     *  |   Return Value   |                       Explanation                      |
     *  |:----------------:|:------------------------------------------------------:|
     *  |               OK | The notifier was attached                              |
     *  |             FAIL | The notifier could not be attached                     |
     *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function          |
     *  |    NOT_SUPPORTED | This function is not supported by the low level driver |
     */
    virtual Chimera::Status_t attachNotifier( const Chimera::Event::Trigger_t event, volatile uint8_t *const notifier )
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    /**
     *   Remove an event notification signal. Should the back end driver decide
     *   to queue multiple notifiers for a given event, the specific notifier is 
     *   passed in. Should this be a nullptr, all registered notifiers will be removed.
     *
     *   @param[in]  event       The event trigger to remove the notifier on
     *   @param[in]  notifier    The notification variable to be removed
     *   @return Chimera::Status_t
     *
     *  |   Return Value   |                       Explanation                      |
     *  |:----------------:|:------------------------------------------------------:|
     *  |               OK | The notifier(s) was removed                            |
     *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function          |
     *  |    NOT_SUPPORTED | This function is not supported by the low level driver |
     */
    virtual Chimera::Status_t detachNotifier( const Chimera::Event::Trigger_t event, volatile uint8_t *const notifier )
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

#if defined( USING_FREERTOS )
    /**
     *  Attach a signal to get notified when an event occurs. All thread safety 
     *  shall performed by FreeRTOS semaphore function calls.
     *
     *  @note Queuing multiple notifiers for an event is optional
     *
     *  @param[in]  event   The event to be notified on
     *  @param[in]  semphr  The notification variable
     *  @return Chimera::Status_t
     *
     *  |   Return Value   |                       Explanation                      |
     *  |:----------------:|:------------------------------------------------------:|
     *  |               OK | The notifier was attached                              |
     *  |             FAIL | The notifier could not be attached                     |
     *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function          |
     *  |    NOT_SUPPORTED | This function is not supported by the low level driver |
     */
    virtual Chimera::Status_t attachNotifier( const Chimera::Event::Trigger_t event, SemaphoreHandle_t *const semphr )
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    /**
     *   Remove an event notification signal. Should the back end driver decide
     *   to queue multiple notifiers for a given event, the specific notifier is 
     *   passed in. Should this be a nullptr, all registered notifiers will be removed.
     *
     *   @param[in]  event   The event to remove the notifier on
     *   @param[in]  semphr  The notification variable
     *   @return Chimera::Status_t
     *
     *  |   Return Value   |                       Explanation                      |
     *  |:----------------:|:------------------------------------------------------:|
     *  |               OK | Everything worked as expected                          |
     *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function          |
     *  |    NOT_SUPPORTED | This function is not supported by the low level driver |
     */
    virtual Chimera::Status_t detachNotifier( const Chimera::Event::Trigger_t event, SemaphoreHandle_t *const semphr )
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }
#endif
  };

  /**
   * Interface describing how to attach and detach callback functions that will execute
   * when a particular event occurs. This is intended to be used in conjuction with 
   * the Chimera::Event::Notifier class, but it is not required.
   */
  class Callback
  {
  public:
    virtual ~Callback() = default;
    
    /**
     *  Allows the user to assign a single callback function to an event trigger
     *
     *  @warning  If running on bare-metal, this will likely be called inside an ISR. Please keep it small.
     *  @note     If running FreeRTOS, this will be called in normal userland via a high priority thread.
     *
     *  @param[in]  trigger   Which event trigger to attach to
     *  @param[in]  func      User callback function
     *  @return Chimera::Status_t
     *
     *  |  Return Value |                  Explanation                 |
     *  |:-------------:|:--------------------------------------------:|
     *  |            OK | The callback was attached successfully       |
     *  |          FAIL | The callback could not be attached           |
     *  | NOT_SUPPORTED | This behavior is not supported on the driver |
     */
    virtual Chimera::Status_t attachCallback( const Chimera::Event::Trigger_t trigger, const Chimera::Function::void_func_void func )
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    /**
     *  Removes a callback function attached to an event trigger
     *
     *  @param[in]  trigger   Which trigger to remove the callback from
     *  @return Chimera::Status_t
     *
     *  |  Return Value |                  Explanation                 |
     *  |:-------------:|:--------------------------------------------:|
     *  |            OK | The trigger callback was empty or cleared    |
     *  |          FAIL | The operation failed                         |
     *  | NOT_SUPPORTED | This behavior is not supported on the driver |
     */
    virtual Chimera::Status_t detachCallback( const Chimera::Event::Trigger_t trigger )
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }
  };
}  // namespace Chimera::Event


#endif  /* !CHIMERA_EVENT_INTERFACE_HPP */