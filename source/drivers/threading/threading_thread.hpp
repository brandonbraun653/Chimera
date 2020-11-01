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
  static constexpr size_t MAX_NAME_LEN = 16;


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
    Thread( Thread &&other );
    Thread( const Thread & ) = delete;
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
    void initialize( ThreadFunctPtr func, ThreadArg arg, const Priority priority, const size_t stackDepth, const std::string_view name );

    /**
     *  Starts the thread
     *  @return void
     */
    void start();

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
     *  Handle to the OS specific thread type. Allows the user to
     *  execute functions that may be available in the current OS
     *  but not supported by this class.
     *
     *  @return detail::native_thread_handle_type
     */
    detail::native_thread_handle_type native_handle();


    explicit operator bool() const
    {
      return ( mThread ) ? true : false;
    }

  private:
    detail::native_thread mThread;
    ThreadFunctPtr mFunc;
    ThreadArg mFuncArg;
    Priority mPriority;
    size_t mStackDepth;
    std::array<char, MAX_NAME_LEN + 1> mThreadName;
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

  }  // namespace this_thread
}

#endif	/* !CHIMERA_THREADING_THREAD_HPP */
