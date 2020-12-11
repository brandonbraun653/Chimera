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
#include <Chimera/adc>
#include <Chimera/can>
#include <Chimera/clock>
#include <Chimera/common>
#include <Chimera/exti>
#include <Chimera/gpio>
#include <Chimera/pwm>
#include <Chimera/serial>
#include <Chimera/spi>
#include <Chimera/system>
#include <Chimera/thread>
#include <Chimera/timer>
#include <Chimera/uart>
#include <Chimera/usart>
#include <Chimera/usb>
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
  Chimera::ADC::initialize();
  Chimera::CAN::initialize();
  Chimera::Clock::initialize();
  Chimera::EXTI::open();
  Chimera::GPIO::initialize();
  Chimera::PWM::initialize();
  Chimera::Serial::initialize();
  Chimera::SPI::initialize();
  Chimera::Timer::initialize();
  Chimera::UART::initialize();
  Chimera::USART::initialize();
  Chimera::Watchdog::initialize();
  Chimera::USB::Peripheral::initialize();
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


  void delayUntil( const size_t val )
  {
    const size_t currentTick = millis();
    if ( currentTick < val )
    {
      delayMilliseconds( val - currentTick );
    }
  }


  void delayMilliseconds( const size_t val )
  {
    Timer::delayMilliseconds( val );
  }


  void delayMilliseconds( const size_t lastTimeWoken, const size_t val )
  {
    delayUntil( lastTimeWoken + val );
  }


  void delayMicroseconds( const size_t val )
  {
    Timer::delayMicroseconds( val );
  }
}  // namespace Chimera
