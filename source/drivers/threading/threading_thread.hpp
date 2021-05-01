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
  class ITask
  {
  public:
    ~ITask() = default;

    /*-------------------------------------------------------------------------------
    Pure Virtual Methods
    -------------------------------------------------------------------------------*/
    /**
     *  Starts the thread, returning it's generated id.
     *  @return TaskId
     */
    virtual TaskId start() = 0;

    /**
     *  Suspends the thread, assuming it's supported. This is mostly taken from real
     *  time operating systems (FreeRTOS) which can commonly suspend a single thread
     *  from execution.
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
     *  Gets the native thread identifier
     *  @return detail::native_thread_id
     */
    virtual detail::native_thread_id native_id() = 0;


    /*-------------------------------------------------------------------------------
    Concrete Methods
    -------------------------------------------------------------------------------*/
    /**
     *  Registers a task configuration
     *
     *  @param[in]  cfg           Task configuration
     *  @return void
     */
    void create( const TaskConfig &cfg );

    /**
     *  Returns the name of the thread
     *  @return std::string_view
     */
    std::string_view name() const;

    /**
     *  Returns the ID associated with the thread
     *  @return TaskId
     */
    TaskId id() const;

    /**
     *  Assigns a given ID to this thread.
     *
     *  @warning There are no checks for duplication of IDs between threads.
     *
     *  @param[in]  id            The ID being assigned
     *  @return void
     */
    void assignId( const TaskId id );

    /**
     *  Checks to see if this thread and another thread are equal
     *
     *  @param[in]  rhs     The thread being compared against
     *  @return bool
     */
    bool operator=( const ITask &rhs )
    {
      /* clang-format off */
      bool POD_compare =
        ( mTaskConfig.function.type == rhs.mTaskConfig.function.type  ) &&
        ( mTaskConfig.arg           == rhs.mTaskConfig.arg            ) &&
        ( mTaskConfig.priority      == rhs.mTaskConfig.priority       ) &&
        ( mTaskConfig.stackWords    == rhs.mTaskConfig.stackWords     ) &&
        ( this->name()              == rhs.name()                     );
      /* clang-format on */

      bool FUNC_compare = false;
      if ( POD_compare )
      {
        if ( mTaskConfig.function.type == FunctorType::C_STYLE )
        {
          FUNC_compare = ( mTaskConfig.function.callable.pointer == rhs.mTaskConfig.function.callable.pointer );
        }
        else
        {
          FUNC_compare = ( mTaskConfig.function.callable.delegate == rhs.mTaskConfig.function.callable.delegate );
        }
      }

      return POD_compare && FUNC_compare;
    }

    /**
     *  Checks if the thread exists/has been initialized yet
     *  @return bool
     */
    explicit operator bool() const
    {
#if defined( USING_NATIVE_THREADS )
      return !( mNativeThread.get_id() == std::thread::id() );
#elif defined( USING_FREERTOS_THREADS )
      return ( mNativeThread ) ? true : false;
#endif
    }

  protected:
    /*-------------------------------------------------
    Shared State Data
    -------------------------------------------------*/
    bool mRunning;                       /**< Is the thread running? */
    TaskId mTaskId;                      /**< Chimera task identifier */
    TaskConfig mTaskConfig;              /**< Stores settings for how the task was initialized */
    detail::native_thread mNativeThread; /**< Default thread storage type */
  };


  /*-------------------------------------------------------------------------------
  Utilities for a thread to use on itself
  -------------------------------------------------------------------------------*/
  namespace this_thread
  {
    /**
     *  Sets the name of the currently executing thread
     *
     *  @param[in]  name        Name to be set
     *  @return void
     */
    void set_name( const char *name );

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
