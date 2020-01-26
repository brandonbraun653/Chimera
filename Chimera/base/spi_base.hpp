/********************************************************************************
 *   File Name:
 *    spi_base.hpp
 *
 *   Description:
 *    Abstract base class interfaces
 *
 *   2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SPI_ABSTRACT_BASE_HPP
#define CHIMERA_SPI_ABSTRACT_BASE_HPP

/* Chimera Includes*/
#include <Chimera/interface/spi_intf.hpp>
#include <Chimera/base/event_base.hpp>
#include <Chimera/base/threading_base.hpp>

namespace Chimera::SPI
{
  class SPIUnsupported : public HardwareDriverInterface,
                         public Chimera::Event::ListenerUnsupported,
                         public Chimera::Threading::AsyncIOUnsupported,
                         public Chimera::Threading::LockableUnsupported
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

    Chimera::Status_t writeBytes( const void *const txBuffer, size_t length, size_t timeoutMS ) final override
    {
      return Chimera::SPI::Status::FAIL;
    }

    Chimera::Status_t readBytes( void *const rxBuffer, size_t length, size_t timeoutMS ) final override
    {
      return Chimera::SPI::Status::FAIL;
    }

    Chimera::Status_t readWriteBytes( const void *const txBuffer, void *const rxBuffer, size_t length,
                                      size_t timeoutMS ) final override
    {
      return Chimera::SPI::Status::FAIL;
    }

    Chimera::Status_t setPeripheralMode( const Chimera::Hardware::PeripheralMode mode ) final override
    {
      return Chimera::SPI::Status::FAIL;
    }

    Chimera::Status_t setClockFrequency( const size_t freq, const size_t tolerance ) final override
    {
      return Chimera::SPI::Status::FAIL;
    }

    size_t getClockFrequency() final override
    {
      return std::numeric_limits<size_t>::min();
    }
  };
}  // namespace Chimera::SPI

#endif /* !CHIMERA_SPI_ABSTRACT_BASE_HPP */