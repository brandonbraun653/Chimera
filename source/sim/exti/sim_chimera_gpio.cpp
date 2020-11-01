/********************************************************************************
 *  File Name:
 *    sim_chimera_exti.cpp
 *
 *  Description:
 *    EXTI Simulator
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#if defined( CHIMERA_SIMULATOR )

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/exti>

namespace Chimera::EXTI::Backend
{
  Chimera::Status_t registerDriver( Chimera::EXTI::Backend::DriverConfig &registry )
  {
    registry.isSupported  = false;
    registry.getDriver    = nullptr;
    registry.initialize   = nullptr;
    registry.reset        = nullptr;
    return Chimera::Status::NOT_SUPPORTED;
  }
}  // namespace Chimera::EXTI::Backend

#endif  /* CHIMERA_SIMULATOR */