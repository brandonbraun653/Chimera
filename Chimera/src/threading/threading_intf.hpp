/********************************************************************************
 *   File Name:
 *    threading_intf.hpp
 *
 *   Description:
 *    Models the Chimera Threading interfaces
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_THREADING_INTERFACE_HPP
#define CHIMERA_THREADING_INTERFACE_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/event>
#include <Chimera/thread>

namespace Chimera::Threading
{
  class AsyncIOInterface
  {
  public:
    virtual ~AsyncIOInterface() = default;

    /**
     *  Asynchronously waits for the given event to occur before the function
     *  will return. The is accomplished by blocking the current thread.
     *
     *  @param[in]  event       The event upon which to be triggered
     *  @param[in]  timeout     How long to wait for the event to occur
     *  @return Chimera::Status_t
     *
     *  |  Return Value |                   Explanation                   |
     *  |:-------------:|:-----------------------------------------------:|
     *  |            OK | The event occurred within the specified timeout |
     *  |       TIMEOUT | The event did not occur                         |
     *  | NOT_SUPPORTED | The requested event cannot be waited upon       |
     */
    virtual Chimera::Status_t await( const Chimera::Event::Trigger event, const size_t timeout ) = 0;

    /**
     *  A more explicit version of await that allows selecting the threading
     *  primitive on which to block. This could be useful if multiple owners
     *  might unblock a process or if the event generator is nested several
     *  calls deep in the stack.
     *
     *  @param[in]  event       The event upon which to be triggered
     *  @param[in]  notifier    Semaphore to be given to upon the event occurrence
     *  @param[in]  timeout     How long to wait for the event to occur
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t await( const Chimera::Event::Trigger event, Chimera::Threading::BinarySemaphore &notifier,
                                     const size_t timeout ) = 0;
  };

  /**
   *  A generic lock interface that can be attached to objects that need locking
   */
  class LockableInterface
  {
  public:
    virtual ~LockableInterface() = default;

    /**
     *  Attempts to reserve the inheriting object
     *
     *  @note If not using FreeRTOS, this function will ignore the timeout
     *  @warning This function can only run from unprivileged code. Do **not** execute in an ISR.
     *
     *  @param[in]  timeout_mS    If using FreeRTOS, how long to wait for the object to be reserved
     *  @return Chimera::Status_t
     *
     *  | Return Value |                     Explanation                    |
     *  |:------------:|:--------------------------------------------------:|
     *  |           OK | The object was reserved before the timeout expired |
     *  |         FAIL | The object was not reserved                        |
     */
    virtual void lock() = 0;

    /**
     *  Attempts to reserve the inheriting object, but from an ISR safe execution context.
     *
     *  @return Chimera::Status_t
     *
     *  | Return Value |                     Explanation                    |
     *  |:------------:|:--------------------------------------------------:|
     *  |           OK | The object was reserved before the timeout expired |
     *  |         FAIL | The object was not reserved                        |
     */
    virtual void lockFromISR() = 0;

    virtual bool try_lock_for( const size_t timeout ) = 0;

    /**
     *  Attempts to release the inheriting object
     *
     *  @note If not using FreeRTOS, this function will release the lock regardless of who holds it.
     *  @warning This function can only run from unprivileged code. Do **not** execute in an ISR.
     *
     *  @return Chimera::Status_t
     *
     *  | Return Value |         Explanation          |
     *  |:------------:|:----------------------------:|
     *  |           OK | The object was released      |
     *  |         FAIL | The object was not released  |
     */
    virtual void unlock() = 0;

    /**
     *  Attempts to release the inheriting object, but from an ISR safe execution context
     *
     *  @note If not using FreeRTOS, this function will release the lock regardless of who holds it.
     *
     *  @return Chimera::Status_t
     *
     *  | Return Value |         Explanation          |
     *  |:------------:|:----------------------------:|
     *  |           OK | The object was released      |
     *  |         FAIL | The object was not released  |
     */
    virtual void unlockFromISR() = 0;
  };
}  // namespace Chimera::Threading

#endif /* !CHIMERA_THREADING_INTERFACE_HPP */