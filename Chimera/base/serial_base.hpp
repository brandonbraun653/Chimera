/********************************************************************************
 *   File Name:
 *    serial_base.hpp
 *
 *   Description:
 *    Abstract base class interfaces
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SERIAL_ABSTRACT_BASE_HPP
#define CHIMERA_SERIAL_ABSTRACT_BASE_HPP

/* Chimera Includes */
#include <Chimera/base/event_base.hpp>
#include <Chimera/base/threading_base.hpp>

namespace Chimera::Serial
{
  class SerialUnsupported : public Chimera::Serial::HWInterface,
                            public Chimera::Event::ListenerUnsupported,
                            public Chimera::Threading::AsyncIOBaseUnsupported,
                            public Chimera::Threading::LockableUnsupported
  {
  public:
    SerialUnsupported()  = default;
    ~SerialUnsupported() = default;

    Chimera::Status_t assignHW( const uint8_t channel, const IOPins &pins ) final override
    {
      return Chimera::CommonStatusCodes::FAIL;
    }

    Chimera::Status_t begin( const Chimera::Hardware::SubPeripheralMode txMode,
                             const Chimera::Hardware::SubPeripheralMode rxMode ) final override
    {
      return Chimera::CommonStatusCodes::FAIL;
    }

    Chimera::Status_t end() final override
    {
      return Chimera::CommonStatusCodes::FAIL;
    }

    Chimera::Status_t configure( const Config &config ) final override
    {
      return Chimera::CommonStatusCodes::FAIL;
    }

    Chimera::Status_t setBaud( const uint32_t baud ) final override
    {
      return Chimera::CommonStatusCodes::FAIL;
    }

    Chimera::Status_t setMode( const Chimera::Hardware::SubPeripheral periph,
                               const Chimera::Hardware::SubPeripheralMode mode ) final override
    {
      return Chimera::CommonStatusCodes::FAIL;
    }

    Chimera::Status_t write( const uint8_t *const buffer, const size_t length, const uint32_t timeout_mS = 500 ) final override
    {
      return Chimera::CommonStatusCodes::FAIL;
    }

    Chimera::Status_t read( uint8_t *const buffer, const size_t length, const uint32_t timeout_mS = 500 ) final override
    {
      return Chimera::CommonStatusCodes::FAIL;
    }

    Chimera::Status_t flush( const Chimera::Hardware::SubPeripheral periph ) final override
    {
      return Chimera::CommonStatusCodes::FAIL;
    }

    void postISRProcessing() final override
    {
    }
  };
}

#endif  // !CHIMERA_SERIAL_ABSTRACT_BASE_HPP
