/********************************************************************************
*  File Name:
*    thread.hpp
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
#include <Chimera/src/threading/internal_types.hpp>
#include <Chimera/src/threading/types.hpp>

namespace Chimera::Threading
{
  
  constexpr size_t MAX_NAME_LEN = 16;

  class Id
  {
  public:
    Id() = default;
    ~Id() = default;

  };
  
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

    void initialize( ThreadFunctPtr func, ThreadArg arg, const Priority priority, const size_t stackDepth, const std::string_view name );

    void start();

    void join();

    bool joinable();

    Id get_id();

    detail::native_thread_handle_type native_handle();

    static int hardware_concurrency();
  
  private:
    detail::native_thread mThread;
    ThreadFunctPtr mFunc;
    ThreadArg mFuncArg;
    Priority mPriority;
    size_t mStackDepth;
    std::array<char, MAX_NAME_LEN + 1> mThreadName;
  };


  namespace this_thread
  {
    Id get_id();

    void sleep_for( const size_t timeout );

    void sleep_until( const size_t timeout );

    void yield();

  }  // namespace this_thread
}

#endif	/* !CHIMERA_THREADING_THREAD_HPP */
