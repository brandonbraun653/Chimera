/********************************************************************************
 *   File Name:
 *    spi_base.hpp
 *
 *   Description:
 *    Abstract base class interfaces
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SPI_ABSTRACT_BASE_HPP
#define CHIMERA_SPI_ABSTRACT_BASE_HPP

/* C++ Includes */
#include <cstdint>

/* Chimera Includes*/
#include <Chimera/interface/spi_intf.hpp>

namespace Chimera::SPI
{
  class SPIUnsupported : public HWInterface
  {
  public:
    SPIUnsupported()  = default;
    ~SPIUnsupported() = default;

    Chimera::Status_t init( const Chimera::SPI::DriverConfig &setupStruct ) final override
    {
      return Chimera::SPI::Status::FAIL;
    }

    Chimera::Status_t deInit() final override
    {
      return Chimera::SPI::Status::FAIL;
    }

    Chimera::Status_t setChipSelect( const Chimera::GPIO::State value ) final override
    {
      return Chimera::SPI::Status::NOT_SUPPORTED;
    }

    Chimera::Status_t setChipSelectControlMode( const Chimera::SPI::CSMode mode ) final override
    {
      return Chimera::SPI::Status::NOT_INITIALIZED;
    }

    Chimera::Status_t writeBytes( const uint8_t *const txBuffer, size_t length, uint32_t timeoutMS ) final override
    {
      return Chimera::SPI::Status::FAIL;
    }

    Chimera::Status_t readBytes( uint8_t *const rxBuffer, size_t length, uint32_t timeoutMS ) final override
    {
      return Chimera::SPI::Status::FAIL;
    }

    Chimera::Status_t readWriteBytes( const uint8_t *const txBuffer, uint8_t *const rxBuffer, size_t length,
                                      uint32_t timeoutMS ) final override
    {
      return Chimera::SPI::Status::FAIL;
    }

    Chimera::Status_t setPeripheralMode( const Chimera::Hardware::SubPeripheral periph,
                                         const Chimera::Hardware::SubPeripheralMode mode ) final override
    {
      return Chimera::SPI::Status::FAIL;
    }

    Chimera::Status_t setClockFrequency( const uint32_t freq, const uint32_t tolerance ) final override
    {
      return Chimera::SPI::Status::FAIL;
    }

    Chimera::Status_t getClockFrequency( uint32_t &freq ) final override
    {
      return Chimera::SPI::Status::FAIL;
    }
  };
}  // namespace Chimera::SPI

#endif /* !CHIMERA_SPI_ABSTRACT_BASE_HPP */