/********************************************************************************
 *  File Name:
 *    spi_user.hpp
 *
 *  Description:
 *    Implements an interface to create a Chimera SPI peripheral.
 *
 *  2019-2022 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SPI_HPP
#define CHIMERA_SPI_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/source/drivers/peripherals/spi/spi_intf.hpp>
#include <Chimera/source/drivers/peripherals/spi/spi_types.hpp>
#include <Chimera/source/drivers/threading/threading_extensions.hpp>


namespace Chimera::SPI
{
  /*---------------------------------------------------------------------------
  Public Functions
  ---------------------------------------------------------------------------*/
  Chimera::Status_t initialize();
  Chimera::Status_t reset();
  Driver_rPtr getDriver( const Channel channel );

  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  class Driver: public Chimera::Thread::Lockable<Driver>,
                public Chimera::Thread::AsyncIO<Driver>,
                public virtual ISPI
  {
  public:
    using Chimera::Thread::AsyncIO<Driver>::AsyncIO;

    Driver();
    ~Driver();

    Chimera::Status_t init( const Chimera::SPI::DriverConfig &setupStruct );
    Chimera::SPI::DriverConfig getInit();
    Chimera::Status_t deInit();
    Chimera::Status_t setChipSelect( const Chimera::GPIO::State value );
    Chimera::Status_t setChipSelectControlMode( const Chimera::SPI::CSMode mode );
    Chimera::Status_t writeBytes( const void *const txBuffer, const size_t length );
    Chimera::Status_t readBytes( void *const rxBuffer, const size_t length );
    Chimera::Status_t readWriteBytes( const void *const txBuffer, void *const rxBuffer, const size_t length );
    Chimera::Status_t setPeripheralMode( const Chimera::Hardware::PeripheralMode mode );
    Chimera::Status_t setClockFrequency( const size_t freq, const size_t tolerance );
    size_t getClockFrequency();

  private:
    friend Chimera::Thread::Lockable<Driver>;
    friend Chimera::Thread::AsyncIO<Driver>;

    void * mImpl; /**< Instance of the implementer's driver */
  };
}  // namespace Chimera::SPI

#endif /* !CHIMERA_SPI_HPP */
