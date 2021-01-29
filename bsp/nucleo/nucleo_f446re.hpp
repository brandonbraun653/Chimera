/********************************************************************************
 *  File Name:
 *    nucleo_f446re.hpp
 *
 *  Description:
 *    BSP for STM32 Nucleo F446RE
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef BSP_STM32_NUCLEO_F446RE_HPP
#define BSP_STM32_NUCLEO_F446RE_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/gpio>
#include <Chimera/spi>
#include <Chimera/serial>


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
      extern const Chimera::GPIO::PinInit config;

      static constexpr Chimera::GPIO::Pin pin   = 5;
      static constexpr Chimera::GPIO::Port port = Chimera::GPIO::Port::PORTA;
    }  // namespace UserLed2
  }    // namespace GPIO

  /*-------------------------------------------------------------------------------
  SPI Definitions
  -------------------------------------------------------------------------------*/
  namespace SPI
  {
    /*-------------------------------------------------
    SPI 1 -- Morpho Connector 10
    -------------------------------------------------*/
    namespace SPI1
    {
      extern const Chimera::SPI::DriverConfig config;

      static constexpr Chimera::GPIO::Pin sckPin    = 5;
      static constexpr Chimera::GPIO::Port sckPort  = Chimera::GPIO::Port::PORTA;
      static constexpr Chimera::GPIO::Pin mosiPin   = 7;
      static constexpr Chimera::GPIO::Port mosiPort = Chimera::GPIO::Port::PORTA;
      static constexpr Chimera::GPIO::Pin misoPin   = 6;
      static constexpr Chimera::GPIO::Port misoPort = Chimera::GPIO::Port::PORTA;
      static constexpr Chimera::GPIO::Pin csPin     = 12;
      static constexpr Chimera::GPIO::Port csPort   = Chimera::GPIO::Port::PORTA;
    }

    /*-------------------------------------------------
    SPI 2 -- Morpho Connector 10
    -------------------------------------------------*/
    namespace SPI2
    {
      extern const Chimera::SPI::DriverConfig config;

      static constexpr Chimera::GPIO::Pin sckPin    = 10;
      static constexpr Chimera::GPIO::Port sckPort  = Chimera::GPIO::Port::PORTB;
      static constexpr Chimera::GPIO::Pin mosiPin   = 15;
      static constexpr Chimera::GPIO::Port mosiPort = Chimera::GPIO::Port::PORTB;
      static constexpr Chimera::GPIO::Pin misoPin   = 14;
      static constexpr Chimera::GPIO::Port misoPort = Chimera::GPIO::Port::PORTB;
      static constexpr Chimera::GPIO::Pin csPin     = 4;
      static constexpr Chimera::GPIO::Port csPort   = Chimera::GPIO::Port::PORTB;
    }

    /*-------------------------------------------------
    SPI 3 -- Morpho Connector 7
    -------------------------------------------------*/
    namespace SPI3
    {
      extern const Chimera::SPI::DriverConfig config;

      static constexpr Chimera::GPIO::Pin sckPin    = 10;
      static constexpr Chimera::GPIO::Port sckPort  = Chimera::GPIO::Port::PORTC;
      static constexpr Chimera::GPIO::Pin mosiPin   = 12;
      static constexpr Chimera::GPIO::Port mosiPort = Chimera::GPIO::Port::PORTC;
      static constexpr Chimera::GPIO::Pin misoPin   = 11;
      static constexpr Chimera::GPIO::Port misoPort = Chimera::GPIO::Port::PORTC;
      static constexpr Chimera::GPIO::Pin csPin     = 2;
      static constexpr Chimera::GPIO::Port csPort   = Chimera::GPIO::Port::PORTD;
    }
  }

  /*-------------------------------------------------------------------------------
  USART Definitions
  -------------------------------------------------------------------------------*/
  namespace USART
  {
    /*-------------------------------------------------
    Virtual com port connected to the attached STLink
    debug module. This will pipe data out over USB to
    the host PC. Requires proper jumper configuration.
    -------------------------------------------------*/
    namespace ComPort
    {
      extern const Chimera::Serial::Config config;
      extern const Chimera::GPIO::PinInit txConfig;
      extern const Chimera::GPIO::PinInit rxConfig;

      static constexpr Chimera::GPIO::Pin txPin    = 2;
      static constexpr Chimera::GPIO::Port txPort  = Chimera::GPIO::Port::PORTA;
      static constexpr Chimera::GPIO::Pin rxPin    = 3;
      static constexpr Chimera::GPIO::Port rxPort  = Chimera::GPIO::Port::PORTA;
    }
  }
}  // namespace BSP::Nucleo::F446RE

#endif /* !BSP_STM32_NUCLEO_F446RE_HPP */
