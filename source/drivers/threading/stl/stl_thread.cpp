/********************************************************************************
 *  File Name:
 *    stl_thread.cpp
 *
 *  Description:
 *    Implements all classes used in Chimera::Threading for the Generic system
 *
 *  2020-2021 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* STL Includes */
#include <chrono>
#include <thread>
#include <cstring>
#include <stdexcept>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/system>
#include <Chimera/thread>
#include <Chimera/source/drivers/threading/threading_thread_internal.hpp>


#if defined( USING_NATIVE_THREADS )

namespace Chimera::Threading
{
  void startScheduler()
  {
    while ( 1 )
    {
      Chimera::Threading::this_thread::sleep_for( 100 );
    }
  }


  bool sendTaskMsg( const ThreadId id, const ThreadMsg msg, const size_t timeout )
  {
    return false;
  }


  /*-------------------------------------------------
  Ctors/Dtors
  -------------------------------------------------*/
  Thread::Thread() : mFunc( {} ), mThreadId( THREAD_ID_INVALID ), mRunning( false )
  {
    mName.fill( 0 );
  }


  Thread::Thread( Thread &&other ) :
      mNativeThread( std::move( other.mNativeThread ) ), mFunc( other.mFunc ), mPriority( other.mPriority ),
      mStackDepth( other.mStackDepth ), mThreadId( other.mThreadId ), mRunning( mRunning )
  {
    copy_thread_name( other.name() );
  }


  Thread::~Thread()
  {
  }


  /*-------------------------------------------------
  Public Methods
  -------------------------------------------------*/
  void Thread::initialize( ThreadFunctPtr func, ThreadArg arg, const Priority priority, const size_t stackDepth,
                           const std::string_view name )
  {
    /*------------------------------------------------
    Copy the parameters
    ------------------------------------------------*/
    mFunc.type             = FunctorType::C_STYLE;
    mFunc.function.pointer = func;
    mFunc.arg              = arg;
    mPriority              = priority;
    mStackDepth            = stackDepth;
    copy_thread_name( name );
  }


  void Thread::initialize( ThreadDelegate func, ThreadArg arg, const Priority priority, const size_t stackDepth,
                           const std::string_view name )
  {
    /*------------------------------------------------
    Copy the parameters
    ------------------------------------------------*/
    mFunc.type              = FunctorType::DELEGATE;
    mFunc.function.delegate = func;
    mFunc.arg               = arg;
    mPriority               = priority;
    mStackDepth             = stackDepth;
    copy_thread_name( name );
  }

  ThreadId Thread::start()
  {
    /*-------------------------------------------------
    This annoyingly obtuse registration uses lambda
    functions as an ad hoc way to inject the calls.
    -------------------------------------------------*/
    if( mFunc.type == FunctorType::C_STYLE )
    {
      ThreadFunctPtr ptr = mFunc.function.pointer;
      ThreadArg arg      = mFunc.arg;
      mNativeThread      = std::move( std::thread( [ ptr, arg ]() { ( *ptr )( arg ); } ) );
    }
    else // FunctorType::DELEGATE
    {
      ThreadArg arg           = mFunc.arg;
      ThreadDelegate delegate = mFunc.function.delegate;
      mNativeThread           = std::move( std::thread( [ delegate, arg ]() { delegate( arg ); } ) );
    }
    // Ensure this function is handling all cases...
    static_assert( static_cast<size_t>( FunctorType::NUM_OPTIONS ) == 2 );

    mRunning = true;
    return registerThread( std::move( *this ) );
  }


  void Thread::suspend()
  {
    // Not supported
  }


  void Thread::resume()
  {
    // Not supported
  }


  void Thread::join()
  {
    mNativeThread.join();
  }

  bool Thread::joinable()
  {
    return mNativeThread.joinable();
  }

  detail::native_thread_handle_type Thread::native_handle()
  {
    return mNativeThread.native_handle();
  }

  void this_thread::sleep_for( const size_t timeout )
  {
    std::this_thread::sleep_for( std::chrono::milliseconds( timeout ) );
  }

  void this_thread::sleep_until( const size_t timeout )
  {
    auto now = std::chrono::steady_clock::now();
    std::this_thread::sleep_until( now + std::chrono::milliseconds( timeout ) );
  }

  void this_thread::yield()
  {
    std::this_thread::yield();
  }

  void this_thread::suspend()
  {
    // Not Supported
  }


  ThreadId this_thread::id()
  {
    return 0;
  }


  bool this_thread::receiveTaskMsg( ThreadMsg &msg, const size_t timeout )
  {
    return false;
  }

}  // namespace Chimera::Threading

#endif /* USING_GENERIC_SYSTEM */
