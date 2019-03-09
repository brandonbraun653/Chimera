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

#include <Chimera/interface.hpp>

/* Test Framework Includes */
#include <gmock/gmock.h>


class SPIMock : public Chimera::SPI::Interface
{
public:
  MOCK_METHOD0( testfunc, void() );

  MOCK_METHOD1( init, Chimera::Status_t( const Chimera::SPI::Setup & ) );
  MOCK_METHOD0( deInit, Chimera::Status_t() );
  MOCK_METHOD1( setChipSelect, Chimera::Status_t( const Chimera::GPIO::State & ) );
  MOCK_METHOD1( setChipSelectControlMode, Chimera::Status_t( const Chimera::SPI::ChipSelectMode & ) );
  MOCK_METHOD5( writeBytes, Chimera::Status_t( const uint8_t *const, size_t, const bool &, const bool &, uint32_t ) );
  MOCK_METHOD5( readBytes, Chimera::Status_t( uint8_t *const, size_t, const bool &, const bool &, uint32_t ) );
  MOCK_METHOD6( readWriteBytes,
                Chimera::Status_t( const uint8_t *const, uint8_t *const, size_t, const bool &, const bool &, uint32_t ) );
  MOCK_METHOD2( setPeripheralMode,
                Chimera::Status_t( const Chimera::SPI::SubPeripheral &, const Chimera::SPI::SubPeripheralMode & ) );
  MOCK_METHOD1( setClockFrequency, Chimera::Status_t( const uint32_t & ) );
  MOCK_METHOD1( getClockFrequency, Chimera::Status_t( uint32_t *const ) );
};
#endif /* !CHIMERA_SPI_MOCK_HPP */
