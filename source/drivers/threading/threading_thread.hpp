/********************************************************************************
 *  File Name:
 *    threading_thread.hpp
 *
 *  Description:
 *    Thread implementation for Chimera that patterns after the C++ STL
 *
 *  2020-2021 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_THREADING_THREAD_HPP
#define CHIMERA_THREADING_THREAD_HPP

/* STL Includes */
#include <array>
#include <cstdlib>
#include <string>

/* Chimera Includes */
#include <Chimera/source/drivers/threading/threading_detail.hpp>
#include <Chimera/source/drivers/threading/threading_types.hpp>

/* ETL Includes */
#include <etl/delegate.h>

namespace Chimera::Thread
{
  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  /**
   *  Gets a pointer to the thread assigned with the given name
   *
   *  @warning Calling thread.join() after acquiring a pointer to a
   *  different thread will invalidate that pointer. Do not cache
   *  the pointer returned from this method.
   *
   *  @param[in]  name      Name given to the thread upon creation
   *  @return Thread *      Returns nullptr if not found
   */
  Task *getThread( const char *name );
  Task *getThread( const std::string_view &name );
  Task *getThread( const TaskId id );

  /**
   *  Sends the message to the requested task, unblocking it if sleeping.
   *
   *  @note This may be called from any execution context, including ISRs.
   *
   *  @param[in]  id        Which task to send to
   *  @param[in]  msg       The message being sent
   *  @param[in]  timeout   How long to wait for successful sending
   *  @return bool
   */
  bool sendTaskMsg( const TaskId id, const TaskMsg msg, const size_t timeout );


  /*-------------------------------------------------------------------------------
  Classes
  -------------------------------------------------------------------------------*/
  /**
   *  A mostly C++ STL compatible thread class, but optimized for embedded OS environments
   *  that have more stringent requirements on thread creation due to resource limitations.
   */
  class IThread
  {
  public:
    ~IThread() = default;

    /**
     *  Creates a thread from the given function ptr and assigns it an execution
     *  priority level + stack to operate with.
     *
     *  @param[in]  ptr          Function pointer defining what the thread executes
     *  @param[in]  priority      Tells the scheduler where this thread fits in the priority hierarchy
     *  @param[in]  stackDepth    How many bytes to allocate from the heap for this thread's stack
     *  @param[in]  name          User friendly name for identification
     */
    virtual void initialize( TaskFuncPtr ptr, TaskArg arg, const Priority priority, const size_t stackDepth,
                             const std::string_view name ) = 0;

    /**
     *  Creates a thread from the given function delegate and assigns it an execution
     *  priority level + stack to operate with.
     *
     *  @param[in]  delegate      Function pointer defining what the thread executes
     *  @param[in]  priority      Tells the scheduler where this thread fits in the priority hierarchy
     *  @param[in]  stackDepth    How many bytes to allocate from the heap for this thread's stack
     *  @param[in]  name          User friendly name for identification
     */
    virtual void initialize( TaskDelegate delegate, TaskArg arg, const Priority priority, const size_t stackDepth,
                             const std::string_view name ) = 0;

    /**
     *  Assigns a given ID to this thread.
     *
     *  @warning There are no checks for duplication of IDs between threads.
     *
     *  @param[in]  id            The ID being assigned
     *  @return void
     */
    virtual void assignId( const TaskId id ) = 0;

    /**
     *  Starts the thread, returning it's generated id.
     *  @return TaskId
     */
    virtual TaskId start() = 0;

    /**
     *  Suspends the thread, assuming it's supported. This is mostly taken from RTOS's
     *  which can commonly suspend a single thread from execution.
     *
     *  @return void
     */
    virtual void suspend() = 0;

    /**
     *  Resumes a previously suspended thread. Does nothing if thread suspension is
     *  not supported.
     *
     *  @return void
     */
    virtual void resume() = 0;

    /**
     *  Sends a termination task message, then unregisters the thread from the system.
     *
     *  @warning There is no feedback on termination success, so calling this method
     *           can leave dangling threads if the termination message is ignored.
     *
     *  @return void
     */
    virtual void join() = 0;

    /**
     *  Checks if the thread can be joined
     *  @return bool
     */
    virtual bool joinable() = 0;

    /**
     *  Handle to the OS specific thread type. Allows the user to execute functions that
     *  may be available in the current OS but not supported by this class.
     *
     *  @return detail::native_thread_handle_type
     */
    virtual detail::native_thread_handle_type native_handle() = 0;

    /**
     *  Returns the name of the thread
     *  @return std::string_view
     */
    virtual std::string_view name() const = 0;

    /**
     *  Returns the ID associated with the thread
     *  @return TaskId
     */
    virtual TaskId id() const = 0;
  };


  /*-------------------------------------------------------------------------------
  Utilities for a thread to use on itself
  -------------------------------------------------------------------------------*/
  namespace this_thread
  {
    /**
     *  Sleeps the current thread for a number of milliseconds
     *
     *  @param[in]  timeout     How many milliseconds to wait
     *  @return void
     */
    void sleep_for( const size_t timeout );

    /**
     *  Sleeps the current thread until an absolute time in the future
     *
     *  @param[in]  timeout     System time to wake up at in milliseconds
     *  @return void
     */
    void sleep_until( const size_t timeout );

    /**
     *  Yields execution to another thread.
     *  @return void
     */
    void yield();

    /**
     *  Instructs the scheduler to place this thread in the suspended state
     *
     *  @warning Once this function is called, another thread must resume this thread
     *  @return void
     */
    void suspend();

    /**
     *  Gets the system identifier of the current thread
     *  @return TaskId
     */
    TaskId id();

    /**
     *  Receives the latest task message for the thread, optionally blocking
     *  for a specified amount of time.
     *
     *  @param[out] msg         The message received, if valid
     *  @param[in]  timeout     How long to wait for a new message
     *  @return bool
     */
    bool receiveTaskMsg( TaskMsg &msg, const size_t timeout );

    /**
     *  Blocks the current thread until the requested task message is received
     *
     *  @param[in]  msg         The message expected to be received
     *  @return bool
     */
    bool pendTaskMsg( TaskMsg msg );

    /**
     *  Blocks the current thread until the requested task message is received
     *  or a timeout expires.
     *
     *  @param[in]  msg         The message expected to be received
     *  @param[in]  timeout     How long to wait for the message
     *  @return bool
     */
    bool pendTaskMsg( TaskMsg msg, const size_t timeout );

  }  // namespace this_thread
}  // namespace Chimera::Thread

#endif /* !CHIMERA_THREADING_THREAD_HPP */
