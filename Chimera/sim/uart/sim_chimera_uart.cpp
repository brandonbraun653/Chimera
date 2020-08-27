/********************************************************************************
 *  File Name:
 *    sim_chimera_uart.cpp
 *
 *  Description:
 *    UART Simulator Driver
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/uart>

namespace Chimera::UART
{
  namespace Backend
  {
    Chimera::Status_t registerDriver( struct Chimera::UART::Backend::DriverConfig &cfg )
    {
      cfg.isSupported   = false;
      cfg.getDriver     = nullptr;
      cfg.initialize    = nullptr;
      cfg.isChannelUART = nullptr;
      cfg.reset         = nullptr;

      return Chimera::Status::OK;
    }
  }  // namespace Backend
}  // namespace Chimera::UART
