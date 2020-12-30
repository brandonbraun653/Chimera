/********************************************************************************
 *  File Name:
 *    threading_thread.cpp
 *
 *  Description:
 *    Common threading implementation
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* STL Includes */
#include <cstdlib>

/* ETL Includes */
#include <etl/flat_map.h>
#include <etl/vector.h>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/system>
#include <Chimera/thread>
#include <Chimera/source/drivers/threading/threading_thread_internal.hpp>

namespace Chimera::Threading
{
  /*-------------------------------------------------------------------------------
  Constants
  -------------------------------------------------------------------------------*/
  static constexpr ThreadId THREAD_ID_REG_MAX = THREAD_ID_INVALID - 1;
  static constexpr ThreadId THREAD_ID_REG_MIN = THREAD_ID_REG_MAX - 1000;
  static constexpr ThreadId THREAD_ID_REG_RNG = THREAD_ID_REG_MAX - THREAD_ID_REG_MIN;

  // Arbitrarily chosen to allow random ID generation to find free IDs quickly
  static_assert( THREAD_ID_REG_RNG > ( 5 * MAX_REGISTERABLE_THREADS ) );

  // Prevent accidental ID range overflow
  static_assert( ( THREAD_ID_REG_MIN + THREAD_ID_REG_RNG ) < THREAD_ID_INVALID );

  /*-------------------------------------------------------------------------------
  Static Data
  -------------------------------------------------------------------------------*/
  static RecursiveMutex s_registry_lock;
  static etl::flat_map<ThreadId, Thread, MAX_REGISTERABLE_THREADS> s_thread_registry;

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
   *  @return ThreadId
   */
  static ThreadId generateId()
  {
    /*-------------------------------------------------
    Thread lock is sufficient as no ISR should be able
    to write to the registry.
    -------------------------------------------------*/
    s_registry_lock.lock();

    /*-------------------------------------------------
    Find an id that doesn't exist in the registry yet
    -------------------------------------------------*/
    ThreadId id;
    while ( true )
    {
      id = ( rand() % THREAD_ID_REG_RNG ) + THREAD_ID_REG_MIN;
      if ( s_thread_registry.find( id ) == s_thread_registry.end() )
      {
        break;
      }
    }

    s_registry_lock.unlock();
    return id;
  }


  /*-------------------------------------------------------------------------------
  Internal Functions
  -------------------------------------------------------------------------------*/
  ThreadId registerThread( Thread &&thread )
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
    ThreadId id = THREAD_ID_INVALID;
    auto msk    = exclusive_lock();
    {
      id = generateId();
      thread.assignId( id );
      s_thread_registry.insert( { id, std::move( thread ) } );
    }
    exclusive_unlock( msk );

    return id;
  }


  Chimera::Status_t unregisterThread( const ThreadId id )
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


  ThreadId getIdFromNativeHandle( detail::native_thread_handle_type handle )
  {
    ThreadId id = THREAD_ID_INVALID;

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


  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  Thread *getThread( const char *name )
  {
    return getThread( std::string_view( name ) );
  }


  Thread *getThread( const std::string_view &name )
  {
    Thread *result = nullptr;
    auto msk       = exclusive_lock();
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


  Thread *getThread( const ThreadId id )
  {
    Thread *result = nullptr;
    auto msk       = exclusive_lock();
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
  void Thread::assignId( const ThreadId id )
  {
    mThreadId = id;
  }


  std::string_view Thread::name() const
  {
    return std::string_view( mName.cbegin() );
  }


  void Thread::copy_thread_name( const std::string_view &name )
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
  }

}  // namespace Chimera::Threading
