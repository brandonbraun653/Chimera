/********************************************************************************
 *   File Name:
 *    threading_intf.hpp
 *
 *   Description:
 *    Models the Chimera Threading interfaces
 *
 *   2019-2022 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_THREADING_INTERFACE_HPP
#define CHIMERA_THREADING_INTERFACE_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/source/drivers/common/common_types.hpp>
#include <Chimera/source/drivers/event/event_types.hpp>
#include <Chimera/source/drivers/threading/threading_semaphore.hpp>
#include <cstdlib>

namespace Chimera::Thread
{
  /*-----------------------------------------------------------------------------
  Classes
  -----------------------------------------------------------------------------*/
  /**
   * @brief Asynchronous notification interface
   * @warning Expects to be used as a SPSC type of signaling mechanism
   *
   * Highly useful for signaling that a particular event has happened, such as
   * a USART transaction has completed, or DMA operation is finished.
   */
  class AsyncIOInterface
  {
  public:
    virtual ~AsyncIOInterface() = default;

    /**
     * @brief Wait on an event, or timeout.
     *
     * Asynchronously waits for the given event to occur before the function
     * will return. The is accomplished by blocking the current thread.
     *
     * @param event       The event upon which to be triggered
     * @param timeout     How long to wait for the event to occur
     * @return Chimera::Status_t
     */
    virtual Chimera::Status_t await( const Chimera::Event::Trigger event, const size_t timeout ) = 0;

    /**
     * @brief Block on a specific signal and await an event, or timeout.
     *
     * A more explicit version of await that allows selecting the threading
     * primitive on which to block. This could be useful if multiple owners
     * might unblock a process or if the event generator is nested several
     * calls deep in the stack.
     *
     * @param event       The event upon which to be triggered
     * @param notifier    Semaphore to be given to upon the event occurrence
     * @param timeout     How long to wait for the event to occur
     * @return Chimera::Status_t
     */
    virtual Chimera::Status_t await( const Chimera::Event::Trigger event, Chimera::Thread::BinarySemaphore &notifier,
                                     const size_t timeout ) = 0;

    /**
     * @brief Signal that a particular event has occurred
     *
     * @param trigger   Which event occurred
     */
    virtual void signalAIO( const Chimera::Event::Trigger trigger ) = 0;
  };


  /**
   *  @brief A generic lock interface that can be attached to objects that need locking
   */
  class LockableInterface
  {
  public:
    virtual ~LockableInterface() = default;

    /**
     * @brief Reserve the inheriting object, blocking if not immediately successful.
     *
     * @warning This function can only run from unprivileged code. Do **not** execute in an ISR.
     * @return void
     */
    virtual void lock() = 0;

    /**
     * @brief Attempts to reserve the inheriting object, but from an ISR safe execution context.
     * @return void
     */
    virtual void lockFromISR() = 0;

    /**
     * @brief Tries to lock the resource for the given amount of time
     *
     * @param timeout     How long to wait to acquire the lock in milliseconds
     * @return bool
     */
    virtual bool try_lock_for( const size_t timeout ) = 0;

    /**
     * @brief Release the inheriting object, assuming current thread has ownership
     *
     * @warning This function can only run from unprivileged code. Do **not** execute in an ISR.
     * @return void
     */
    virtual void unlock() = 0;

    /**
     * @brief Release the inheriting object, but from an ISR safe execution context
     * @return void
     */
    virtual void unlockFromISR() = 0;
  };
}  // namespace Chimera::Thread

#endif /* !CHIMERA_THREADING_INTERFACE_HPP */