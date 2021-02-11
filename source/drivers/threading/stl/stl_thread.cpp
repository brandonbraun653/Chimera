/********************************************************************************
 *  File Name:
 *    stl_thread.cpp
 *
 *  Description:
 *    Implements all classes used in Chimera::Thread for the Generic system
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
#include <Chimera/source/drivers/threading/common/threading_internal.hpp>


#if defined( USING_NATIVE_THREADS )

namespace Chimera::Thread
{
  void startScheduler()
  {
    while ( 1 )
    {
      Chimera::Thread::this_thread::sleep_for( 100 );
    }
  }


  bool sendTaskMsg( const TaskId id, const TaskMsg msg, const size_t timeout )
  {
    return false;
  }


  /*-------------------------------------------------
  Ctors/Dtors
  -------------------------------------------------*/
  Task::Task() : mFunc( {} ), mTaskId( THREAD_ID_INVALID ), mRunning( false )
  {
    mName.fill( 0 );
  }


  Task::Task( Task &&other ) :
      mNativeThread( std::move( other.mNativeThread ) ), mFunc( other.mFunc ), mPriority( other.mPriority ),
      mStackDepth( other.mStackDepth ), mTaskId( other.mTaskId ), mRunning( mRunning )
  {
    copy_thread_name( other.name() );
  }


  Task::~Task()
  {
  }


  /*-------------------------------------------------
  Public Methods
  -------------------------------------------------*/
  void Task::initialize( TaskFuncPtr func, TaskArg arg, const Priority priority, const size_t stackDepth,
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


  void Task::initialize( TaskDelegate func, TaskArg arg, const Priority priority, const size_t stackDepth,
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

  TaskId Task::start()
  {
    /*-------------------------------------------------
    This annoyingly obtuse registration uses lambda
    functions as an ad hoc way to inject the calls.
    -------------------------------------------------*/
    if( mFunc.type == FunctorType::C_STYLE )
    {
      TaskFuncPtr ptr = mFunc.function.pointer;
      TaskArg arg      = mFunc.arg;
      mNativeThread      = std::move( std::thread( [ ptr, arg ]() { ( *ptr )( arg ); } ) );
    }
    else // FunctorType::DELEGATE
    {
      TaskArg arg           = mFunc.arg;
      TaskDelegate delegate = mFunc.function.delegate;
      mNativeThread           = std::move( std::thread( [ delegate, arg ]() { delegate( arg ); } ) );
    }
    // Ensure this function is handling all cases...
    static_assert( static_cast<size_t>( FunctorType::NUM_OPTIONS ) == 2 );

    mRunning = true;
    return registerThread( std::move( *this ) );
  }


  void Task::suspend()
  {
    // Not supported
  }


  void Task::resume()
  {
    // Not supported
  }


  void Task::join()
  {
    mNativeThread.join();
  }

  bool Task::joinable()
  {
    return mNativeThread.joinable();
  }

  detail::native_thread_handle_type Task::native_handle()
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


  TaskId this_thread::id()
  {
    return 0;
  }


  bool this_thread::receiveTaskMsg( TaskMsg &msg, const size_t timeout )
  {
    return false;
  }

}  // namespace Chimera::Thread

#endif /* USING_GENERIC_SYSTEM */
