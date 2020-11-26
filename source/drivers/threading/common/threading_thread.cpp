/********************************************************************************
 *  File Name:
 *    threading_thread.cpp
 *
 *  Description:
 *    Common threading implementation
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* ETL Includes */
#include <etl/vector.h>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/thread>

namespace Chimera::Threading
{
  /*-------------------------------------------------------------------------------
  Static Data
  -------------------------------------------------------------------------------*/
  static RecursiveMutex s_registry_lock;
  static etl::vector<Thread, MAX_REGISTERABLE_THREADS> s_thread_registry;

  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  Chimera::Status_t registerThread( Thread &&thread )
  {
    if ( s_thread_registry.full() )
    {
      return Chimera::Status::FULL;
    }

    s_registry_lock.lock();
    s_thread_registry.push_back( std::move( thread ) );
    s_registry_lock.unlock();

    return Chimera::Status::OK;
  }


  Chimera::Status_t unregisterThread( const char *name )
  {
    std::string_view tmpName( name );
    auto result = Chimera::Status::NOT_FOUND;

    s_registry_lock.lock();
    for ( auto x = 0; x < s_thread_registry.size(); x++ )
    {
      if ( s_thread_registry[ x ].name() == tmpName )
      {
        s_thread_registry.erase( s_thread_registry.begin() + x );
        result = Chimera::Status::OK;
        break;
      }
    }
    s_registry_lock.unlock();

    return result;
  }


  Thread *findThread( const char *name )
  {
    return findThread( std::string_view( name ) );
  }


  Thread *findThread( const std::string_view &name )
  {
    Thread *result = nullptr;

    s_registry_lock.lock();
    for ( auto x = 0; x < s_thread_registry.size(); x++ )
    {
      if ( s_thread_registry[ x ].name() == name )
      {
        result = &s_thread_registry[ x ];
        break;
      }
    }
    s_registry_lock.unlock();

    return result;
  }


  /*-------------------------------------------------------------------------------
  Class Definition
  -------------------------------------------------------------------------------*/
  std::string_view Thread::name() const
  {
    return std::string_view( mThreadName.cbegin() );
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

    mThreadName.fill( 0 );
    memcpy( mThreadName.data(), name.data(), copyLen );
  }

}  // namespace Chimera::Threading
