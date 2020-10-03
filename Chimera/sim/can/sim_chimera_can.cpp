/********************************************************************************
 *  File Name:
 *    sim_chimera_can.cpp
 *
 *  Description:
 *    CAN Simulator
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#if defined( CHIMERA_SIMULATOR )

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/can>

namespace Chimera::CAN::Backend
{
  Chimera::Status_t registerDriver( Chimera::CAN::Backend::DriverConfig &registry )
  {
    registry.isSupported  = false;
    registry.getDriver    = nullptr;
    registry.initialize   = nullptr;
    registry.reset        = nullptr;
    return Chimera::Status::NOT_SUPPORTED;
  }
}  // namespace Chimera::CAN::Backend

#endif  /* CHIMERA_SIMULATOR */