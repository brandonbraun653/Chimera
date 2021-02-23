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
#include <Chimera/assert>
#include <Chimera/common>
#include <Chimera/system>
#include <Chimera/thread>
#include <Chimera/source/drivers/threading/common/threading_internal.hpp>

#if defined( USING_NATIVE_THREADS )

namespace Chimera::Thread
{
  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  void startScheduler()
  {
    while ( 1 )
    {
      Chimera::Thread::this_thread::sleep_for( 100 );
    }
  }


  bool sendTaskMsg( const TaskId id, const TaskMsg msg, const size_t timeout )
  {
    auto thread = getThread( id );
    RT_HARD_ASSERT( thread );
    return thread->acceptTaskMessage( msg, timeout );
  }


  /*-------------------------------------------------
  Ctors/Dtors
  -------------------------------------------------*/
  Task::Task()
  {
    mTaskConfig       = {};
    mTaskId           = Chimera::Thread::THREAD_ID_INVALID;
    mRunning          = false;
    mTaskMsgMutex     = new std::mutex();
    mTaskMsgCondition = new std::condition_variable();
  }


  Task::Task( Task &&other ) : mTaskMsgMutex( other.mTaskMsgMutex ), mTaskMsgCondition( other.mTaskMsgCondition )
  {
    mRunning         = other.mRunning;
    mTaskId          = other.mTaskId;
    mTaskConfig      = other.mTaskConfig;
    mNativeThread    = std::move( other.mNativeThread );
    mTaskConfig.name = other.mTaskConfig.name;
  }


  Task::~Task()
  {
  }


  /*-------------------------------------------------
  Public Methods
  -------------------------------------------------*/
  void Task::initialize( TaskFuncPtr func, TaskArg arg, const Priority priority, const size_t stackWords,
                         const std::string_view name )
  {
    /*------------------------------------------------
    Copy the parameters
    ------------------------------------------------*/
    mTaskConfig.function.type             = FunctorType::C_STYLE;
    mTaskConfig.function.callable.pointer = func;
    mTaskConfig.arg                       = arg;
    mTaskConfig.priority                  = priority;
    mTaskConfig.stackWords                = stackWords;
    mTaskConfig.name                      = name.cbegin();
  }


  void Task::initialize( TaskDelegate func, TaskArg arg, const Priority priority, const size_t stackWords,
                         const std::string_view name )
  {
    /*------------------------------------------------
    Copy the parameters
    ------------------------------------------------*/
    mTaskConfig.function.type              = FunctorType::DELEGATE;
    mTaskConfig.function.callable.delegate = func;
    mTaskConfig.arg                        = arg;
    mTaskConfig.priority                   = priority;
    mTaskConfig.stackWords                 = stackWords;
    mTaskConfig.name                       = name.cbegin();
  }


  TaskId Task::start()
  {
    /*-------------------------------------------------
    ***Developer Note***
    The STL threading implementation does not care
    about static/dynamic/restricted task types, so they
    are not handled here
    -------------------------------------------------*/

    /*-------------------------------------------------
    This annoyingly obtuse registration uses lambda
    functions as an ad hoc way to inject the calls.
    -------------------------------------------------*/
    std::string_view name{ mTaskConfig.name.data() };

    if ( mTaskConfig.function.type == FunctorType::C_STYLE )
    {
      TaskFuncPtr ptr = mTaskConfig.function.callable.pointer;
      TaskArg arg     = mTaskConfig.arg;

      mNativeThread = std::move( std::thread( [ ptr, arg, name ]() {
        this_thread::set_name( name.begin() );
        ( *ptr )( arg );
      } ) );
    }
    else  // FunctorType::DELEGATE
    {
      TaskArg arg           = mTaskConfig.arg;
      TaskDelegate delegate = mTaskConfig.function.callable.delegate;
      mNativeThread         = std::move( std::thread( [ delegate, arg, name ]() {
        this_thread::set_name( name.begin() );
        delegate( arg );
      } ) );
    }
    // Ensure this function is handling all cases...
    static_assert( static_cast<size_t>( FunctorType::NUM_OPTIONS ) == 2 );

    mRunning = true;
    return registerThread( std::move( *this ) );
  }


  void Task::suspend()
  {
    // Not Supported
    RT_HARD_ASSERT( false );
  }


  void Task::resume()
  {
    // Not Supported
    RT_HARD_ASSERT( false );
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


  bool Task::acceptTaskMessage( const TaskMsg msg, const size_t timeout )
  {
    /*-------------------------------------------------
    Update the data and signal waiting threads
    -------------------------------------------------*/
    std::lock_guard<std::mutex> lk( *mTaskMsgMutex, std::adopt_lock_t() );
    mTaskMsgData  = msg;
    mTaskMsgReady = true;
    mTaskMsgCondition->notify_one();

    return true;
  }


  bool Task::pendTaskMessage( TaskMsg &msg, const size_t timeout )
  {
    std::unique_lock<std::mutex> lk( *mTaskMsgMutex, std::adopt_lock_t() );
    bool received = mTaskMsgCondition->wait_for( lk, std::chrono::milliseconds( timeout ), [ this ] { return this->mTaskMsgReady; } );

    if( received )
    {
      msg           = mTaskMsgData;
      mTaskMsgReady = false;
    }

    return received;
  }

  detail::native_thread_id Task::native_id()
  {
    return mNativeThread.get_id();
  }


  /*-------------------------------------------------------------------------------
  Namespace this_thread Implementation
  -------------------------------------------------------------------------------*/
  void this_thread::yield()
  {
    std::this_thread::yield();
  }


  void this_thread::suspend()
  {
    // Not Supported
    RT_HARD_ASSERT( false );
  }


  TaskId this_thread::id()
  {
    std::thread::id id = std::this_thread::get_id();
    return getIdFromNativeId( id );
  }


  bool this_thread::receiveTaskMsg( TaskMsg &msg, const size_t timeout )
  {
    TaskId id   = this_thread::id();
    auto thread = getThread( id );
    RT_HARD_ASSERT( thread );
    return thread->pendTaskMessage( msg, timeout );
  }

}  // namespace Chimera::Thread

#endif /* USING_GENERIC_SYSTEM */
