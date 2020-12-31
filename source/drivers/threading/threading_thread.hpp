/********************************************************************************
 *  File Name:
 *    threading_thread.hpp
 *
 *  Description:
 *    Thread implementation for Chimera that patterns after the C++ STL
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
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

namespace Chimera::Threading
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
  Thread *getThread( const char *name );
  Thread *getThread( const std::string_view &name );
  Thread *getThread( const ThreadId id );

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
  bool sendTaskMsg( const ThreadId id, const ThreadMsg msg, const size_t timeout );


  /*-------------------------------------------------------------------------------
  Classes
  -------------------------------------------------------------------------------*/
  /**
   *  A mostly C++ STL compatible thread class, but optimized for embedded OS environments
   *  that have more stringent requirements on thread creation due to resource limitations.
   */
  class Thread
  {
  public:
    Thread();
    Thread( const Thread & );
    Thread( Thread &&other );
    // Thread( const Thread & ) = delete;
    ~Thread();

    /**
     *  Creates a thread from the given function pointer and assigns it an execution
     *  priority level + stack to operate with.
     *
     *  @param[in]  func          Function pointer defining what the thread executes
     *  @param[in]  priority      Tells the scheduler where this thread fits in the priority hierarchy
     *  @param[in]  stackDepth    How many bytes to allocate from the heap for this thread's stack
     *  @param[in]  name          User friendly name for identification
     */
    void initialize( ThreadFunctPtr func, ThreadArg arg, const Priority priority, const size_t stackDepth,
                     const std::string_view name );
    void initialize( ThreadDelegate func, ThreadArg arg, const Priority priority, const size_t stackDepth,
                     const std::string_view name );

    /**
     *  Assigns a given ID to this thread.
     *
     *  @warning There are no checks for duplication of IDs between threads.
     *
     *  @param[in]  id            The ID being assigned
     *  @return void
     */
    void assignId( const ThreadId id );

    /**
     *  Starts the thread, returning it's generated id.
     *  @return ThreadId
     */
    ThreadId start();

    /**
     *  Suspends the thread, assuming it's supported. This is mostly taken from RTOS's
     *  which can commonly suspend a single thread from execution.
     *
     *  @return void
     */
    void suspend();

    /**
     *  Resumes a previously suspended thread. Does nothing if thread suspension is
     *  not supported.
     *
     *  @return void
     */
    void resume();

    /**
     *  Sends a termination task message, then unregisters the thread from the system.
     *
     *  @warning There is no feedback on termination success, so calling this method
     *           can leave dangling threads if the termination message is ignored.
     *
     *  @return void
     */
    void join();

    /**
     *  Checks if the thread can be joined
     *  @return bool
     */
    bool joinable();

    /**
     *  Handle to the OS specific thread type. Allows the user to xecute functions that
     *  may be available in the current OS but not supported by this class.
     *
     *  @return detail::native_thread_handle_type
     */
    detail::native_thread_handle_type native_handle();

    /**
     *  Returns the name of the thread
     *  @return std::string_view
     */
    std::string_view name() const;

    /**
     *  Returns the ID associated with the thread
     *  @return ThreadId
     */
    ThreadId id() const
    {
      return mThreadId;
    }

    /**
     *  Checks if the thread exists/has been initialized yet
     *  @return bool
     */
    explicit operator bool() const
    {
      return ( mNativeThread ) ? true : false;
    }

    bool operator=( const Thread &rhs )
    {
      /* clang-format off */
      bool POD_compare =
        ( this->mFunc.type == rhs.mFunc.type ) &&
        ( this->mFunc.arg == rhs.mFunc.arg ) &&
        ( this->mPriority == rhs.mPriority ) &&
        ( this->mStackDepth == rhs.mStackDepth ) &&
        ( this->name() == rhs.name() );
      /* clang-format on */

      bool FUNC_compare = false;
      if ( POD_compare )
      {
        if ( this->mFunc.type == FunctorType::C_STYLE )
        {
          FUNC_compare = ( this->mFunc.function.pointer == rhs.mFunc.function.pointer );
        }
        else
        {
          FUNC_compare = ( this->mFunc.function.delegate == rhs.mFunc.function.delegate );
        }
      }

      return POD_compare && FUNC_compare;
    }

  private:
    bool mRunning;
    detail::native_thread mNativeThread;
    UserFunction mFunc;
    ThreadId mThreadId;
    Priority mPriority;
    size_t mStackDepth;
    std::array<char, MAX_NAME_LEN + 1> mName;

    void lookup_handle();
    void copy_thread_name( const std::string_view &name );
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
     *  @return ThreadId
     */
    ThreadId id();

    /**
     *  Receives the latest task message for the thread, optionally blocking
     *  for a specified amount of time.
     *
     *  @param[out] msg         The message received, if valid
     *  @param[in]  timeout     How long to wait for a new message
     *  @return bool
     */
    bool receiveTaskMsg( ThreadMsg &msg, const size_t timeout );

    /**
     *  Blocks the current thread until the requested task message is received
     *
     *  @param[in]  msg         The message expected to be received
     *  @return bool
     */
    bool pendTaskMsg( ThreadMsg msg );

    /**
     *  Blocks the current thread until the requested task message is received
     *  or a timeout expires.
     *
     *  @param[in]  msg         The message expected to be received
     *  @param[in]  timeout     How long to wait for the message
     *  @return bool
     */
    bool pendTaskMsg( ThreadMsg msg, const size_t timeout );

  }  // namespace this_thread
}  // namespace Chimera::Threading

#endif /* !CHIMERA_THREADING_THREAD_HPP */
