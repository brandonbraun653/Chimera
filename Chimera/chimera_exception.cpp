/********************************************************************************
 *   File Name:
 *    exception.cpp
 *
 *   Description:
 *    Lightweight exception signaling implementation for Chimera
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */
#include <atomic>

#include <Chimera/exception.hpp>

namespace Chimera
{
  namespace Exception
  {
    static std::atomic<bool> outOfRangeException;

    void initialize()
    {
      outOfRangeException = false;
    }

    bool out_of_range_thrown()
    {
      return outOfRangeException;
    }

    void out_of_range_clear()
    {
      outOfRangeException = false;
    }
  }  // namespace Exception
}  // namespace Chimera


#if defined( BOOST_CONTAINER_USER_DEFINED_THROW_CALLBACKS )
#include <boost/throw_exception.hpp>

namespace boost
{
  namespace container
  {
    void throw_bad_alloc()
    {
    }

    void throw_out_of_range( const char *str )
    {
      Chimera::Exception::outOfRangeException = true;
    }

    void throw_length_error( const char *str )
    {
    }

    void throw_logic_error( const char *str )
    {
    }

    void throw_runtime_error( const char *str )
    {
    }
  }  // namespace container
}  // namespace boost
#endif /* BOOST_CONTAINER_USER_DEFINED_THROW_CALLBACKS */
