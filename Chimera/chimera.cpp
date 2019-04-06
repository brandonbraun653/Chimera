/********************************************************************************
 *   File Name:
 *    chimera.cpp
 *
 *   Description:
 *      Implements common Chimera functions
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#include "chimeraPort.hpp"
#include <Chimera/chimera.hpp>
#include <Chimera/exception.hpp>
#include <Chimera/preprocessor.hpp>
#include <Chimera/threading.hpp>

#if defined( SIM )
/* Boost Includes */
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#endif

void ChimeraInit()
{
#ifdef MOD_TEST

#else
  // Backend namespace has class that inherits from chimera::System::interface or something
  // and then has static member functions that perform initialization. Do the same for the
  // timer and delay functionality.
  cSystemInit();
#endif

  Chimera::Exception::initialize();
}

namespace Chimera
{
  uint32_t millis()
  {
#ifdef BACKEND_NAMESPACE
    return BACKEND_NAMESPACE::millis();
#else
    return 0u;
#endif
  }

  void delayMilliseconds( uint32_t ms )
  {
#ifdef BACKEND_NAMESPACE
    BACKEND_NAMESPACE::delayMilliseconds( ms );
#else
    boost::this_thread::sleep_for( boost::chrono::milliseconds( ms ) );
#endif
  }

  void delayMicroseconds( uint32_t us )
  {
#ifdef BACKEND_NAMESPACE
    BACKEND_NAMESPACE::delayMicroseconds( us );
#else
    boost::this_thread::sleep_for( boost::chrono::microseconds( us ) );
#endif
  }
}  // namespace Chimera
