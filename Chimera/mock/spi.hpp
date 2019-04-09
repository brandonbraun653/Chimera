/********************************************************************************
 * File Name:
 *	  spi.hpp
 *
 * Description:
 *	  Implements a mock version of the Chimera SPI Driver
 *
 * 2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SPI_MOCK_HPP
#define CHIMERA_SPI_MOCK_HPP

/* C++ Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/interface.hpp>

#if defined( GMOCK_TEST )
/* Test Framework Includes */
#include <gmock/gmock.h>

namespace Chimera
{
  namespace Mock
  {
    class SPIMock : public Chimera::SPI::Interface
    {
    public:
      MOCK_METHOD1( init, Chimera::Status_t( const Chimera::SPI::Setup & ) );
      MOCK_METHOD0( deInit, Chimera::Status_t() );
      MOCK_METHOD1( setChipSelect, Chimera::Status_t( const Chimera::GPIO::State & ) );
      MOCK_METHOD1( setChipSelectControlMode, Chimera::Status_t( const Chimera::SPI::ChipSelectMode & ) );
      MOCK_METHOD3( writeBytes, Chimera::Status_t( const uint8_t *const, const size_t, const uint32_t ) );
      MOCK_METHOD3( readBytes, Chimera::Status_t( uint8_t *const, const size_t, const uint32_t ) );
      MOCK_METHOD4( readWriteBytes, Chimera::Status_t( const uint8_t *const, uint8_t *const, const size_t, const uint32_t ) );
      MOCK_METHOD2( setPeripheralMode,
                    Chimera::Status_t( const Chimera::SPI::SubPeripheral, const Chimera::SPI::SubPeripheralMode ) );
      MOCK_METHOD2( setClockFrequency, Chimera::Status_t( const uint32_t, const uint32_t ) );
      MOCK_METHOD1( getClockFrequency, Chimera::Status_t( uint32_t & ) );
      MOCK_METHOD1( reserve, Chimera::Status_t( const uint32_t ) );
      MOCK_METHOD1( release, Chimera::Status_t( const uint32_t ) );
      MOCK_METHOD1( onWriteCompleteCallback, Chimera::Status_t( const Chimera::void_func_uint32_t ) );
      MOCK_METHOD1( onReadCompleteCallback, Chimera::Status_t( const Chimera::void_func_uint32_t ) );
      MOCK_METHOD1( onReadWriteCompleteCallback, Chimera::Status_t( const Chimera::void_func_uint32_t ) );
      MOCK_METHOD1( onErrorCompleteCallback, Chimera::Status_t( const Chimera::void_func_uint32_t ) );
    };

    using SPIMock_sPtr = std::shared_ptr<SPIMock>;
    using SPIMock_uPtr = std::unique_ptr<SPIMock>;
  }  // namespace Mock
}  // namespace Chimera

#endif /* GMOCK_TEST */

#endif /* !CHIMERA_SPI_MOCK_HPP */
