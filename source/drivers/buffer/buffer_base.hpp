/********************************************************************************
 *   File Name:
 *    buffer_base.hpp
 *
 *   Description:
 *    Abstract base class interfaces
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_BUFFER_ABSTRACT_BASE_HPP
#define CHIMERA_BUFFER_ABSTRACT_BASE_HPP

/* STL Includes */
#include <cstdint>

/* Chimera Includes*/
#include <Chimera/source/drivers/buffer/buffer_intf.hpp>

namespace Chimera::Buffer
{
  class DoubleBufferableUnsupported : public DoubleBufferableInterface
  {
  public:
    DoubleBufferableUnsupported()  = default;
    ~DoubleBufferableUnsupported() = default;

    Chimera::Status_t bufferPush( const Chimera::Hardware::SubPeripheral periph, uint8_t *const buffer, const size_t len )
    {
      return Chimera::Status::NOT_SUPPORTED;
    }

    Chimera::Status_t bufferPop( const Chimera::Hardware::SubPeripheral periph, uint8_t *const buffer, const size_t len )
    {
      return Chimera::Status::NOT_SUPPORTED;
    }

    Chimera::Status_t bufferEnable( const Chimera::Hardware::SubPeripheral periph )
    {
      return Chimera::Status::NOT_SUPPORTED;
    }

    Chimera::Status_t bufferDisable( const Chimera::Hardware::SubPeripheral periph )
    {
      return Chimera::Status::NOT_SUPPORTED;
    }

    Chimera::Status_t bufferAssignInternal( const Chimera::Hardware::SubPeripheral periph, uint8_t *const buffer,
                                            const uint16_t size )
    {
      return Chimera::Status::NOT_SUPPORTED;
    }

    Chimera::Status_t bufferAssignExternal( const Chimera::Hardware::SubPeripheral periph,
                                            Chimera::Serial::CircularBuffer & buffer )
    {
      return Chimera::Status::NOT_SUPPORTED;
    }

    Chimera::Status_t bufferFlush( const Chimera::Hardware::SubPeripheral periph )
    {
      return Chimera::Status::NOT_SUPPORTED;
    }
  };
}  // namespace Chimera::Buffer

#endif /* !CHIMERA_BUFFER_ABSTRACT_BASE_HPP */