/********************************************************************************
 *  File Name:
 *    threading_thread.cpp
 *
 *  Description:
 *    Common threading implementation
 *
 *  2020-2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#if defined( __linux__ )
#include <sys/prctl.h>
#endif /* __linux__ */

/* STL Includes */
#include <cstdlib>

/* ETL Includes */
#include <etl/flat_map.h>
#include <etl/vector.h>

/* Chimera Includes */
#include <Chimera/assert>
#include <Chimera/common>
#include <Chimera/system>
#include <Chimera/thread>
#include <Chimera/source/drivers/threading/common/threading_internal.hpp>

namespace Chimera::Thread
{
  /*-------------------------------------------------------------------------------
  Constants
  -------------------------------------------------------------------------------*/
  static constexpr TaskId THREAD_ID_REG_MAX = 1000;
  static constexpr TaskId THREAD_ID_REG_MIN = 100;
  static constexpr TaskId THREAD_ID_REG_RNG = THREAD_ID_REG_MAX - THREAD_ID_REG_MIN;

  // Arbitrarily chosen to allow random ID generation to find free IDs quickly
  static_assert( THREAD_ID_REG_RNG > ( 5 * MAX_REGISTERABLE_THREADS ) );

  // Prevent accidental ID range overflow
  static_assert( ( THREAD_ID_REG_MIN + THREAD_ID_REG_RNG ) < THREAD_ID_INVALID );

  /*-------------------------------------------------------------------------------
  Static Data
  -------------------------------------------------------------------------------*/
  static RecursiveMutex s_registry_lock;
  static etl::flat_map<TaskId, Task, MAX_REGISTERABLE_THREADS> s_thread_registry;

  // It might be worth it to add a message queue for each thread in some kind of struct?
  // This would get around the FreeRTOS style queue, but doesn't do a great job of passing
  // larger amounts of data. It would however unify the message passing interface, which
  // could be useful I guess.

  /*-------------------------------------------------------------------------------
  Static Functions
  -------------------------------------------------------------------------------*/
  /**
   *  Locks thread resources from thread access and interruption
   *  from any system interrupts.
   *  @return Chimera::System::InterruptMask
   */
  static Chimera::System::InterruptMask exclusive_lock()
  {
    s_registry_lock.lock();
    return Chimera::System::disableInterrupts();
  }

  /**
   *  Restores interrupt and thread access to thread resources
   *  @return void
   */
  static void exclusive_unlock( Chimera::System::InterruptMask msk )
  {
    Chimera::System::enableInterrupts( msk );
    s_registry_lock.unlock();
  }

  /**
   *  Generates a pseudo-random ID for thread identification. Will not
   *  conflict with any thread IDs that are currently in use.
   *
   *  @return TaskId
   */
  static TaskId generateId()
  {
    static TaskId s_registry_id_index = THREAD_ID_REG_MIN;

    /*-------------------------------------------------------------------------
    The best ID for a thread is the next one!
    -------------------------------------------------------------------------*/
    s_registry_lock.lock();
    TaskId id = s_registry_id_index++;
    s_registry_lock.unlock();

    return id;
  }


  /*-------------------------------------------------------------------------------
  Internal Functions
  -------------------------------------------------------------------------------*/
  TaskId registerThread( Task &&thread )
  {
    /*-------------------------------------------------
    Input protection
    -------------------------------------------------*/
    if ( s_thread_registry.full() )
    {
      Chimera::insert_debug_breakpoint();
      return THREAD_ID_INVALID;
    }

    /*-------------------------------------------------
    Insert the new thread object while protecting from
    all possible interferences.
    -------------------------------------------------*/
    TaskId id = THREAD_ID_INVALID;
    auto msk  = exclusive_lock();
    {
      id = generateId();
      thread.assignId( id );
      s_thread_registry.insert( { id, std::move( thread ) } );
    }
    exclusive_unlock( msk );

    return id;
  }


  Chimera::Status_t unregisterThread( const TaskId id )
  {
    auto result = Chimera::Status::NOT_FOUND;
    auto msk    = exclusive_lock();
    {
      if ( auto iter = s_thread_registry.find( id ); iter != s_thread_registry.end() )
      {
        s_thread_registry.erase( iter );
        result = Chimera::Status::OK;
      }
    }
    exclusive_unlock( msk );

    return result;
  }


  TaskId getIdFromNativeHandle( detail::native_thread_handle_type handle )
  {
    TaskId id = THREAD_ID_INVALID;

    auto msk = exclusive_lock();
    {
      for ( auto iter = s_thread_registry.begin(); iter != s_thread_registry.end(); iter++ )
      {
        if ( iter->second.native_handle() == handle )
        {
          id = iter->second.id();
          break;
        }
      }
    }
    exclusive_unlock( msk );

    return id;
  }


  TaskId getIdFromNativeId( const detail::native_thread_id id )
  {
    TaskId foundId = THREAD_ID_INVALID;

    auto msk = exclusive_lock();
    {
      for ( auto iter = s_thread_registry.begin(); iter != s_thread_registry.end(); iter++ )
      {
        if ( iter->second.native_id() == id )
        {
          foundId = iter->second.id();
          break;
        }
      }
    }
    exclusive_unlock( msk );

#if defined( DEBUG )
    RT_HARD_ASSERT( foundId != THREAD_ID_INVALID );
#endif /* DEBUG */

    return foundId;
  }


  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  Task *getThread( const char *name )
  {
    return getThread( std::string_view( name ) );
  }


  Task *getThread( const std::string_view &name )
  {
    Task *result = nullptr;
    auto msk     = exclusive_lock();
    {
      for ( auto iter = s_thread_registry.begin(); iter != s_thread_registry.end(); iter++ )
      {
        if ( iter->second.name() == name )
        {
          result = &iter->second;
          break;
        }
      }
    }
    exclusive_unlock( msk );

    return result;
  }


  Task *getThread( const TaskId id )
  {
    Task *result = nullptr;
    auto msk     = exclusive_lock();
    {
      if ( auto iter = s_thread_registry.find( id ); iter != s_thread_registry.end() )
      {
        result = &iter->second;
      }
    }
    exclusive_unlock( msk );

    return result;
  }


  /*-------------------------------------------------------------------------------
  Class Definition
  -------------------------------------------------------------------------------*/
  void ITask::create( const TaskConfig &cfg )
  {
    mTaskConfig = cfg;
  }


  TaskId ITask::id() const
  {
    return mTaskId;
  }


  void ITask::assignId( const TaskId id )
  {
    mTaskId = id;
  }


  std::string_view ITask::name() const
  {
    return std::string_view( mTaskConfig.name.cbegin() );
  }

}  // namespace Chimera::Thread


namespace Chimera::Thread::this_thread
{
  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  void set_name( const char *name )
  {
    /*-------------------------------------------------
    Only need to handle Linux here. The FreeRTOS version
    will pick up the name from the task creation.
    -------------------------------------------------*/
#if defined( __linux__ )
    prctl( PR_SET_NAME, name, 0, 0, 0 );
#endif
  }


  TaskName get_name()
  {
    /*-------------------------------------------------
    Need a way to pull the current task context in a
    generic fashion...hmm...
    -------------------------------------------------*/
    Chimera::insert_debug_breakpoint();

    return TaskName();
  }


  bool pendTaskMsg( TaskMsg msg )
  {
    return pendTaskMsg( msg, Chimera::Thread::TIMEOUT_BLOCK );
  }


  bool pendTaskMsg( TaskMsg msg, const size_t timeout )
  {
    TaskMsg actMsg = std::numeric_limits<TaskMsg>::max();
    return ( receiveTaskMsg( actMsg, timeout ) && ( actMsg == msg ) );
  }


  void sleep_for( const size_t timeout )
  {
    Chimera::delayMilliseconds( timeout );
  }


  void sleep_until( const size_t timeout )
  {
    auto now = Chimera::millis();
    if ( timeout > now )
    {
      Chimera::delayMilliseconds( timeout - now );
    }
  }

}  // namespace Chimera::Thread::this_thread
