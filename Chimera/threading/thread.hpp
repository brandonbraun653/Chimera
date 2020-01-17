/********************************************************************************
*  File Name:
*    thread.hpp
*
*  Description:
*    Thread implementation for Chimera that patterns after the C++ STL
*
*  2020 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/

#ifndef CHIMERA_THREADING_THREAD_HPP
#define CHIMERA_THREADING_THREAD_HPP

/* C++ Includes */
#include <array>
#include <cstdlib>
#include <string>

/* Chimera Includes */
#include <Chimera/interface/threading_intf.hpp>
#include <Chimera/threading/internal_types.hpp>
#include <Chimera/threading/types.hpp>

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
    Thread( ThreadFunctPtr func, ThreadArg arg );
    Thread( const Thread & ) = delete;
    ~Thread();

    void start( const Priority priority, const size_t stackDepth, const std::string_view name );

    void join();

    bool joinable();

    Id get_id();

    detail::native_thread_handle_type native_handle();

    static int hardware_concurrency();
  
  private:
    detail::native_thread mThread;
    const ThreadFunctPtr mFunc;
    const ThreadArg mFuncArg;
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
