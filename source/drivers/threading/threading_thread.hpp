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

namespace Chimera::Threading
{
  /*-------------------------------------------------------------------------------
  Constants
  -------------------------------------------------------------------------------*/
  static constexpr size_t MAX_NAME_LEN             = 16;
  static constexpr size_t MAX_REGISTERABLE_THREADS = 5;


  /*-------------------------------------------------------------------------------
  Forward Declarations
  -------------------------------------------------------------------------------*/
  class Thread;


  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  /**
   *  Registers a thread with the backend such that it can be
   *  referenced or manipulated later.
   *
   *  @param[in]  thread      The thread to copy
   *  @return Chimera::Status_t
   */
  Chimera::Status_t registerThread( Thread &&thread );

  /**
   *  Removes a thread from the registry
   *
   *  @param[in]  name      Name of the thread
   *  @return Chimera::Status_t
   */
  Chimera::Status_t unregisterThread( const char *name );

  /**
   *  Gets a pointer to the thread assigned with the given name. Must have
   *  been registered first.
   *
   *  @warning Calling unregisterThread() after acquiring a pointer to a
   *  pointer to a different thread will invalidate that pointer.
   *
   *  @param[in]  name      Name given to the thread upon creation
   *  @return Thread *      Returns nullptr if not found
   */
  Thread *findThread( const char *name );
  Thread *findThread( const std::string_view &name );

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

    /**
     *  Starts the thread
     *  @return void
     */
    void start();

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
     *  Waits for the thread to finish execution and return
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
     *  Checks if the thread exists/has been initialized yet
     *  @return bool
     */
    explicit operator bool() const
    {
      return ( mThread ) ? true : false;
    }

    bool operator=( const Thread &rhs )
    {
      /* clang-format off */
      return (
        ( this->mFunc == rhs.mFunc ) &&
        ( this->mFuncArg == rhs.mFuncArg ) &&
        ( this->mPriority == rhs.mPriority ) &&
        ( this->mStackDepth == rhs.mStackDepth ) &&
        ( this->name() == rhs.name() )
      );
      /* clang-format on */
    }

  private:
    detail::native_thread mThread;
    ThreadFunctPtr mFunc;
    ThreadArg mFuncArg;
    Priority mPriority;
    size_t mStackDepth;
    std::array<char, MAX_NAME_LEN + 1> mThreadName;

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

  }  // namespace this_thread
}  // namespace Chimera::Threading

#endif /* !CHIMERA_THREADING_THREAD_HPP */
