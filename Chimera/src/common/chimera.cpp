/********************************************************************************
 *  File Name:
 *    chimera.cpp
 *
 *  Description:
 *      Implements common Chimera functions
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Chimera Includes */
#include <Chimera/clock>
#include <Chimera/common>
#include <Chimera/gpio>
#include <Chimera/pwm>
#include <Chimera/serial>
#include <Chimera/spi>
#include <Chimera/system>
#include <Chimera/thread>
#include <Chimera/timer>
#include <Chimera/uart>
#include <Chimera/usart>
#include <Chimera/watchdog>


void ChimeraInit()
{
  /*------------------------------------------------
  Initialize the back end driver first so that all the necessary
  resources can be initialized properly
  ------------------------------------------------*/
  Chimera::System::initialize();
  Chimera::System::systemStartup();

  /*------------------------------------------------
  Initialize peripheral drivers, which hook into the backend
  ------------------------------------------------*/
  Chimera::Clock::initialize();
  Chimera::GPIO::initialize();
  Chimera::PWM::initialize();
  Chimera::Serial::initialize();
  Chimera::SPI::initialize();
  Chimera::Timer::initialize();
  Chimera::UART::initialize();
  Chimera::USART::initialize();
  Chimera::Watchdog::initialize();
}

namespace Chimera
{
  size_t millis()
  {
    return Timer::millis();
  }

  size_t micros()
  {
    return Timer::micros();
  }

  void delayMilliseconds( const size_t val )
  {
    Timer::delayMilliseconds( val );
  }

  void delayMicroseconds( const size_t val )
  {
    Timer::delayMicroseconds( val );
  }
}  // namespace Chimera
