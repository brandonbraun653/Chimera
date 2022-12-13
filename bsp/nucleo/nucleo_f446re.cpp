/******************************************************************************
 *  File Name:
 *    nucleo_f446re.cpp
 *
 *  Description:
 *    BSP implementation detail
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

/* Package Includes */
#include <Chimera/bsp/nucleo/nucleo_f446re.hpp>

namespace BSP::Nucleo::F446RE
{
  /*---------------------------------------------------------------------------
  GPIO Definitions
  ---------------------------------------------------------------------------*/
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
  }  // namespace GPIO

  /*---------------------------------------------------------------------------
  USART
  ---------------------------------------------------------------------------*/
  namespace USART
  {
    namespace ComPort
    {
      const Chimera::Serial::Config config = { .baud     = 115200,
                                               .width    = Chimera::Serial::CharWid::CW_8BIT,
                                               .parity   = Chimera::Serial::Parity::PAR_NONE,
                                               .stopBits = Chimera::Serial::StopBits::SBITS_ONE,
                                               .flow     = Chimera::Serial::FlowControl::FCTRL_NONE };

      const Chimera::GPIO::PinInit txConfig = { .alternate = Chimera::GPIO::Alternate::USART2_TX,
                                                .drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL,
                                                .pin       = txPin,
                                                .port      = txPort,
                                                .pull      = Chimera::GPIO::Pull::NO_PULL,
                                                .state     = Chimera::GPIO::State::HIGH,
                                                .threaded  = false,
                                                .validity  = true };

      const Chimera::GPIO::PinInit rxConfig = { .alternate = Chimera::GPIO::Alternate::USART2_RX,
                                                .drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL,
                                                .pin       = rxPin,
                                                .port      = rxPort,
                                                .pull      = Chimera::GPIO::Pull::NO_PULL,
                                                .state     = Chimera::GPIO::State::HIGH,
                                                .threaded  = false,
                                                .validity  = true };
    }  // namespace ComPort
  }    // namespace USART
}  // namespace BSP::Nucleo::F446RE
