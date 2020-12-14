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
  Lockable::Lockable()
  {
  }

  Lockable::~Lockable()
  {
  }

  void Lockable::lock()
  {
    mutex.lock();
  }

  void Lockable::lockFromISR()
  {
    mutex.lock();
  }

  bool Lockable::try_lock_for( const size_t timeout )
  {
    return mutex.try_lock_for( timeout );
  }

  void Lockable::unlock()
  {
    mutex.unlock();
  }

  void Lockable::unlockFromISR()
  {
    mutex.unlock();
  }


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

  Thread::Thread() : mFunc( nullptr ), mFuncArg( nullptr )
  {
    mName.fill( 0 );
  }

  Thread::Thread( Thread &&other ) : mFunc( other.mFunc ), mFuncArg( other.mFuncArg )
  {
    mName.fill( 0 );
  }

  Thread::~Thread()
  {
  }

  void Thread::initialize( ThreadFunctPtr func, ThreadArg arg, const Priority priority, const size_t stackDepth,
                           const std::string_view name )
  {
    /*------------------------------------------------
      Copy out the string data into the name
      ------------------------------------------------*/
    size_t copyLen = name.length();
    if ( copyLen > MAX_NAME_LEN )
    {
      copyLen = MAX_NAME_LEN;
    }

    mName.fill( 0 );
    memcpy( mName.data(), name.data(), copyLen );

    /*------------------------------------------------
    Copy the additional parameters
    ------------------------------------------------*/
    mFunc       = func;
    mFuncArg    = arg;
    mPriority   = priority;
    mStackDepth = stackDepth;
  }

  void Thread::start()
  {
    mNativeThread = std::thread( mFunc, mFuncArg );
  }


  void Thread::suspend()
  {
    throw std::runtime_error( "Thread suspension not supported" );
  }


  void Thread::resume()
  {
    // Not supported since suspend() isn't.
  }


  void Thread::join()
  {
    mNativeThread.join();
  }

  bool Thread::joinable()
  {
    return mNativeThread.joinable();
  }

  Id Thread::get_id()
  {
    // auto stl_id = mNativeThread.get_id();

    return Id();
  }

  detail::native_thread_handle_type Thread::native_handle()
  {
    return mNativeThread.native_handle();
  }

  int Thread::hardware_concurrency()
  {
    return Chimera::System::maxConcurrentThreads();
  }

  Id this_thread::get_id()
  {
    // auto stl_id = std::this_thread::get_id();

    return Id();
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
    throw std::runtime_error( "Thread suspension not supported" );
  }

}  // namespace Chimera::Threading

#endif /* USING_GENERIC_SYSTEM */
