/********************************************************************************
 *  File Name:
 *    sim_chimera_system.cpp
 *
 *  Description:
 *    Simulates Chimera system functionality
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/system.hpp>

namespace Chimera::System
{
  Chimera::Status_t prjSystemStartup()
  {
    return Chimera::CommonStatusCodes::OK;
  }
}  // namespace Chimera::System
