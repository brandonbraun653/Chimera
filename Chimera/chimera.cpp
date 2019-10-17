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

namespace Chimera
{
  size_t millis()
  {
    return BACKEND_NAMESPACE::millis();
  }

  void delayMilliseconds( size_t ms )
  {
    BACKEND_NAMESPACE::delayMilliseconds( ms );
  }

  void delayMicroseconds( size_t us )
  {
    BACKEND_NAMESPACE::delayMicroseconds( us );
  }
}  // namespace Chimera
