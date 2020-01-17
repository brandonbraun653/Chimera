/********************************************************************************
 *  File Name:
 *    sim_chimera_system.cpp
 *
 *  Description:
 *    Simulates Chimera system functionality
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#if defined( _WIN32 ) || defined( _WIN64 )

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/system.hpp>

namespace Chimera::System
{
  Chimera::Status_t prjSystemStartup()
  {
    return Chimera::CommonStatusCodes::OK;
  }

  InterruptMask prjDisableInterrupts()
  {
    InterruptMask temp;
    temp.interrupted = true;

    return temp;
  }

  void prjEnableInterrupts( InterruptMask &interruptMask )
  {
    interruptMask.interrupted = false;
  }

  int prjMaxConcurrentThreads()
  {
    return -1;
  }
}  // namespace Chimera::System

#endif /* _WIN32 || _WIN64 */
