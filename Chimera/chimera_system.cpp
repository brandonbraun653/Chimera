/********************************************************************************
 *   File Name:
 *    chimera_system.cpp
 *
 *   Description:
 *    Implements high level system interfaces for Chimera
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */


/* Chimera Includes */
#include <Chimera/system.hpp>
#include <Chimera/interface/system_intf.hpp>

namespace Chimera::System
{
  static InterruptMask s_SystemInterruptState;

  Chimera::Status_t initialize()
  {
    memset( &s_SystemInterruptState, 0, sizeof( s_SystemInterruptState ) );

    /*------------------------------------------------
    Execute the user's initialization code first so that Chimera
    has a clean base to work from.
    ------------------------------------------------*/
    if ( prjSystemStartup() != Chimera::CommonStatusCodes::OK )
    {
      return Chimera::CommonStatusCodes::FAIL;
    }

    /*------------------------------------------------
    Perform other initialization steps...when needed...
    ------------------------------------------------*/

    return Chimera::CommonStatusCodes::OK;
  }

  void disableInterrupts()
  {
    s_SystemInterruptState = prjDisableInterrupts();
  }

  void enableInterrupts()
  {
    if ( s_SystemInterruptState.interrupted )
    {
      prjEnableInterrupts( s_SystemInterruptState );
    }
  }

  int maxConcurrentThreads()
  {
    return prjMaxConcurrentThreads();
  }
}