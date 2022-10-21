/******************************************************************************
 *  File Name:
 *    i2c_user.hpp
 *
 *  Description:
 *    Chimera I2C User Interface
 *
 *  2021-2022 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_I2C_USER_INTERFACE_HPP
#define CHIMERA_I2C_USER_INTERFACE_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/source/drivers/peripherals/i2c/i2c_intf.hpp>
#include <Chimera/source/drivers/peripherals/i2c/i2c_types.hpp>
#include <Chimera/source/drivers/threading/threading_extensions.hpp>


namespace Chimera::I2C
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
  class Driver : public Chimera::Thread::Lockable<Driver>,
                 public Chimera::Thread::AsyncIO<Driver>,
                 public virtual II2C
  {
  public:
    using Chimera::Thread::AsyncIO<Driver>::AsyncIO;

    Driver();
    ~Driver();

    /**
     * @brief Opens an I2C port using the given configuration
     *
     * @param cfg   The I2C configuration settings
     * @return Chimera::Status_t
     */
    Chimera::Status_t open( const DriverConfig &cfg );

    /**
     * @brief Closes the current port if already open.
     *
     * @return Chimera::Status_t
     */
    Chimera::Status_t close();

    /**
     * @brief Reads data off the bus
     *
     * @param address   Address to read from
     * @param data      Data buffer to read into
     * @param length    Number of bytes to read
     * @return Chimera::Status_t
     */
    Chimera::Status_t read( const uint16_t address, void *const data, const size_t length );

    /**
     * @brief Writes data onto the bus
     *
     * @param address   Address to write to
     * @param data      Data buffer to write from
     * @param length    Number of bytes to write
     * @return Chimera::Status_t
     */
    Chimera::Status_t write( const uint16_t address, const void *const data, const size_t length );

    /**
     * @brief Perform a full-duplex transfer on the bus
     *
     * @param address   Address to read/write from
     * @param tx_data   Data buffer to write from
     * @param rx_data   Data buffer to read into
     * @param length    Number of bytes to transfer
     * @return Chimera::Status_t
     */
    Chimera::Status_t transfer( const uint16_t address, const void *const tx_data, void *const rx_data,
                                        const size_t length );
    /**
     * @brief Generate a stop event on the bus
     *
     * @return Chimera::Status_t
     */
    Chimera::Status_t stop();

    /**
     * @brief Generate a start event on the bus
     *
     * @return Chimera::Status_t
     */
    Chimera::Status_t start();

    /*-------------------------------------------------------------------------
    Interface: Listener
    -------------------------------------------------------------------------*/
    Chimera::Status_t registerListener( Chimera::Event::Actionable &listener, const size_t timeout, size_t &registrationID );
    Chimera::Status_t removeListener( const size_t registrationID, const size_t timeout );

  private:
    friend Chimera::Thread::Lockable<Driver>;
    friend Chimera::Thread::AsyncIO<Driver>;

    void * mImpl; /**< Instance of the implementer's driver */
  };

}  // namespace Chimera::I2C

#endif  /* !CHIMERA_I2C_USER_INTERFACE_HPP */
