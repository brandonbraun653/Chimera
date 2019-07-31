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

#include <Chimera/sim/sim_chimera.hpp>

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
    return BACKEND_NAMESPACE::millis();
  }

  void delayMilliseconds( uint32_t ms )
  {
    BACKEND_NAMESPACE::delayMilliseconds( ms );
  }

  void delayMicroseconds( uint32_t us )
  {
    BACKEND_NAMESPACE::delayMicroseconds( us );
  }
}  // namespace Chimera
