/********************************************************************************
 *  File Name:
 *    sim_chimera_usart.cpp
 *
 *  Description:
 *    USART Simulator Driver
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/usart>

namespace Chimera::USART
{
  namespace Backend
  {
    Chimera::Status_t registerDriver( struct Chimera::USART::Backend::DriverConfig &cfg )
    {
      cfg.isSupported    = false;
      cfg.getDriver      = nullptr;
      cfg.initialize     = nullptr;
      cfg.isChannelUSART = nullptr;
      cfg.reset          = nullptr;

      return Chimera::Status::OK;
    }
  }  // namespace Backend
}  // namespace Chimera::USART
