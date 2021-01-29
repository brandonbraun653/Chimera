/********************************************************************************
 *  File Name:
 *    nucleo_f446re.cpp
 *
 *  Description:
 *    BSP implementation detail
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Package Includes */
#include <Chimera/bsp/nucleo/nucleo_f446re.hpp>

namespace BSP::Nucleo::F446RE
{
  /*-------------------------------------------------------------------------------
  GPIO Definitions
  -------------------------------------------------------------------------------*/
  namespace GPIO
  {
    /*-------------------------------------------------------------------------------
    Green LED
    -------------------------------------------------------------------------------*/
    namespace UserLed2
    {
      const Chimera::GPIO::PinInit config = { .alternate = Chimera::GPIO::Alternate::NONE,
                                              .drive     = Chimera::GPIO::Drive::OUTPUT_PUSH_PULL,
                                              .pin       = pin,
                                              .port      = port,
                                              .pull      = Chimera::GPIO::Pull::NO_PULL,
                                              .state     = Chimera::GPIO::State::LOW,
                                              .threaded  = false,
                                              .validity  = true };
    }
  }    // namespace GPIO
}    // namespace BSP::Nucleo::F446RE
