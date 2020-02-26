/********************************************************************************
 *  File Name:
 *    chimera.cpp
 *
 *  Description:
 *      Implements common Chimera functions
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Chimera Includes */
#include <Chimera/src/common/chimera.hpp>
#include <Chimera/src/common/preprocessor.hpp>
#include <Chimera/system>
#include <Chimera/thread>
#include <Chimera/timer>

void ChimeraInit()
{
  Chimera::System::prjSystemStartup();
}

namespace Chimera
{
  size_t millis()
  {
    return Timer::millis();
  }

  void delayMilliseconds( const size_t val )
  {
    Timer::delayMilliseconds( val );
  }

  void delayMicroseconds( const size_t val )
  {
    Timer::delayMicroseconds( val );
  }
}  // namespace Chimera
