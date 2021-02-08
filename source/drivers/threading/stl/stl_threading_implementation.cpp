/********************************************************************************
 *  File Name:
 *    generic_threading_implementation.cpp
 *
 *  Description:
 *    Implements all classes used in Chimera::Threading for the Generic system.
 *
 *  Notes:
 *    1. This will require the underlying system to have some idea of how to
 *       enable/disable system level interrupts to simulate blocking access to
 *       resources.
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
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


#if defined( USING_WINDOWS ) || defined( USING_LINUX ) || defined( USING_NATIVE_THREADS )

namespace Chimera::Threading
{
  /************************************************************************/
  /*                             EXTENSIONS                               */
  /************************************************************************/


  /************************************************************************/
  /*                              SEMAPHORE                               */
  /************************************************************************/
  CountingSemaphore::CountingSemaphore() : mMaxCount( 1 ), mCount( 1 )
  {
  }

  CountingSemaphore::CountingSemaphore( const size_t maxCounts ) : mMaxCount( maxCounts ), mCount( maxCounts )
  {
  }

  CountingSemaphore::~CountingSemaphore()
  {
  }

  void CountingSemaphore::release( const size_t update )
  {
    semphr.lock();
    auto newVal = mCount + update;
    mCount      = ( newVal <= mMaxCount ) ? newVal : mMaxCount;
    semphr.unlock();
  }

  void CountingSemaphore::acquire()
  {
    semphr.lock();

    if ( mCount > 0 )
    {
      --mCount;
    }
  }

  bool CountingSemaphore::try_acquire()
  {
    if ( semphr.try_lock() && ( mCount > 0 ) )
    {
      --mCount;
      return true;
    }

    return false;
  }

  bool CountingSemaphore::try_acquire_for( const size_t timeout )
  {
    if ( semphr.try_lock_for( std::chrono::milliseconds( timeout ) ) && ( mCount > 0 ) )
    {
      --mCount;
      return true;
    }

    return false;
  }

  bool CountingSemaphore::try_acquire_until( const size_t abs_time )
  {
    auto now = std::chrono::steady_clock::now();
    if ( semphr.try_lock_until( now + std::chrono::milliseconds( abs_time ) ) && ( mCount > 0 ) )
    {
      --mCount;
      return true;
    }

    return false;
  }

  size_t CountingSemaphore::max()
  {
    return mMaxCount;
  }

  void CountingSemaphore::acquireFromISR()
  {
    Chimera::insert_debug_breakpoint();
  }

  void CountingSemaphore::releaseFromISR()
  {
    Chimera::insert_debug_breakpoint();
  }

  /************************************************************************/
  /*                                MUTEX                                 */
  /************************************************************************/

  /*------------------------------------------------
  Mutex Implementation
  ------------------------------------------------*/
  Mutex::Mutex()
  {
  }

  Mutex::~Mutex()
  {
  }

  void Mutex::lock()
  {
    _mtx.lock();
  }

  bool Mutex::try_lock()
  {
    return _mtx.try_lock();
  }

  void Mutex::unlock()
  {
    _mtx.unlock();
  }

  /*------------------------------------------------
  Recursive Mutex Implementation
  ------------------------------------------------*/
  RecursiveMutex::RecursiveMutex()
  {
  }

  RecursiveMutex::~RecursiveMutex()
  {
  }

  void RecursiveMutex::lock()
  {
    _mtx.lock();
  }

  bool RecursiveMutex::try_lock()
  {
    return _mtx.try_lock();
  }

  void RecursiveMutex::unlock()
  {
    _mtx.unlock();
  }

  /*------------------------------------------------
  Timed Mutex Implementation
  ------------------------------------------------*/
  TimedMutex::TimedMutex()
  {
  }

  TimedMutex::~TimedMutex()
  {
  }

  void TimedMutex::lock()
  {
    _mtx.lock();
  }

  bool TimedMutex::try_lock()
  {
    return _mtx.try_lock();
  }

  bool TimedMutex::try_lock_for( const size_t timeout )
  {
    return _mtx.try_lock_for( std::chrono::milliseconds( timeout ) );
  }

  bool TimedMutex::try_lock_until( const size_t timeout )
  {
    auto now = std::chrono::steady_clock::now();
    return _mtx.try_lock_until( now + std::chrono::milliseconds( timeout ) );
  }

  void TimedMutex::unlock()
  {
    _mtx.unlock();
  }

  /*------------------------------------------------
  Recursive Timed Mutex Implementation
  ------------------------------------------------*/
  RecursiveTimedMutex::RecursiveTimedMutex()
  {
  }

  RecursiveTimedMutex::~RecursiveTimedMutex()
  {
  }

  void RecursiveTimedMutex::lock()
  {
    _mtx.lock();
  }

  bool RecursiveTimedMutex::try_lock()
  {
    return _mtx.try_lock();
  }

  bool RecursiveTimedMutex::try_lock_for( const size_t timeout )
  {
    return _mtx.try_lock_for( std::chrono::milliseconds( timeout ) );
  }

  bool RecursiveTimedMutex::try_lock_until( const size_t timeout )
  {
    auto now = std::chrono::steady_clock::now();
    return _mtx.try_lock_until( now + std::chrono::milliseconds( timeout ) );
  }

  void RecursiveTimedMutex::unlock()
  {
    _mtx.unlock();
  }

  /************************************************************************/
  /*                               THREAD                                 */
  /************************************************************************/
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


  Thread::Thread( const Thread &other ) : mFunc( other.mFunc ), mPriority( other.mPriority ),
      mStackDepth( other.mStackDepth ), mThreadId( other.mThreadId ), mRunning( mRunning )
  {
    copy_thread_name( other.name() );
  }


  Thread::Thread( Thread &&other ) : mFunc( other.mFunc ), mPriority( other.mPriority ),
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
    return 0;
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
