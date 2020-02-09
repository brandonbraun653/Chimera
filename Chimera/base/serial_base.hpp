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
#include <Chimera/interface/serial_intf.hpp>

namespace Chimera::Serial
{
  class SerialUnsupported : public Chimera::Serial::HWInterface,
                            public Chimera::Event::ListenerUnsupported,
                            public Chimera::Threading::AsyncIOUnsupported,
                            public Chimera::Threading::LockableUnsupported
  {
  public:
    SerialUnsupported()  = default;
    ~SerialUnsupported() = default;

    Chimera::Status_t assignHW( const uint8_t channel, const IOPins &pins ) final override
    {
      return Chimera::CommonStatusCodes::FAIL;
    }

    Chimera::Status_t begin( const Chimera::Hardware::PeripheralMode txMode,
                             const Chimera::Hardware::PeripheralMode rxMode ) final override
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
                               const Chimera::Hardware::PeripheralMode mode ) final override
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

    Chimera::Status_t readAsync( uint8_t *const buffer, const size_t len ) final override
    {
      return Chimera::CommonStatusCodes::FAIL;
    }

    Chimera::Status_t enableBuffering( const Chimera::Hardware::SubPeripheral periph,
                                       boost::circular_buffer<uint8_t> *const userBuffer, uint8_t *const hwBuffer,
                                       const uint32_t hwBufferSize ) final override
    {
      return Chimera::CommonStatusCodes::FAIL;
    }

    Chimera::Status_t disableBuffering( const Chimera::Hardware::SubPeripheral periph ) final override
    {
      return Chimera::CommonStatusCodes::FAIL;
    }

    bool available( size_t *const bytes ) final override
    {
      return false;
    }
  };
}

#endif  // !CHIMERA_SERIAL_ABSTRACT_BASE_HPP
