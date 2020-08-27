/********************************************************************************
 *  File Name:
 *    sim_chimera_watchdog.cpp
 *
 *  Description:
 *    Watchdog Simulator
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#if defined( CHIMERA_SIMULATOR )

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/watchdog>

namespace Chimera::Watchdog::Backend
{
  Chimera::Status_t registerDriver( Chimera::Watchdog::Backend::DriverConfig &registry )
  {
    registry.isSupported  = false;
    registry.createShared = nullptr;
    registry.createUnique = nullptr;
    registry.initialize   = nullptr;
    registry.reset        = nullptr;
    return Chimera::Status::NOT_SUPPORTED;
  }
}  // namespace Chimera::Watchdog::Backend

#endif /* CHIMERA_SIMULATOR */