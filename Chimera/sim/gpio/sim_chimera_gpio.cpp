/********************************************************************************
 *  File Name:
 *    sim_chimera_gpio.cpp
 *
 *  Description:
 *    GPIO Simulator
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#if defined( CHIMERA_SIMULATOR )

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/gpio>

namespace Chimera::GPIO::Backend
{
  Chimera::Status_t registerDriver( Chimera::GPIO::Backend::DriverConfig &registry )
  {
    registry.isSupported  = false;
    registry.getDriver    = nullptr;
    registry.initialize   = nullptr;
    registry.reset        = nullptr;
    return Chimera::Status::NOT_SUPPORTED;
  }
}  // namespace Chimera::GPIO::Backend

#endif  /* CHIMERA_SIMULATOR */