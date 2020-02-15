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
#include <Chimera/sim/sim_chimera.hpp>
#include <Chimera/system>
#include <Chimera/thread>

#include "chimeraPort.hpp"

void ChimeraInit()
{
  Chimera::System::prjSystemStartup();
}


namespace Chimera
{
  size_t millis()
  {
    return BACKEND_NAMESPACE::millis();
  }

  void delayMilliseconds( const size_t val )
  {
    BACKEND_NAMESPACE::delayMilliseconds( val );
  }

  void delayMicroseconds( const size_t val )
  {
    BACKEND_NAMESPACE::delayMicroseconds( val );
  }
}  // namespace Chimera
