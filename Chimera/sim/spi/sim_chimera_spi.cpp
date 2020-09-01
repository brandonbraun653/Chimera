/********************************************************************************
 *  File Name:
 *    sim_chimera_spi.cpp
 *
 *  Description:
 *    SPI Simulator
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#if defined( CHIMERA_SIMULATOR )

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/spi>

namespace Chimera::SPI::Backend
{
  Chimera::Status_t registerDriver( Chimera::SPI::Backend::DriverConfig &registry )
  {
    registry.isSupported  = false;
    registry.getDriver    = nullptr;
    registry.initialize   = nullptr;
    registry.reset        = nullptr;
    return Chimera::Status::NOT_SUPPORTED;
  }
}  // namespace Chimera::SPI::Backend

#endif /* CHIMERA_SIMULATOR */