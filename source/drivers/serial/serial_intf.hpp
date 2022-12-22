/******************************************************************************
 *  File Name:
 *    serial_intf.hpp
 *
 *  Description:
 *    Models the Chimera Serial interface for UART/USARTs
 *
 *  2019-2022 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_SERIAL_INTERFACE_HPP
#define CHIMERA_SERIAL_INTERFACE_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/callback>
#include <Chimera/common>
#include <Chimera/event>
#include <Chimera/source/drivers/serial/serial_types.hpp>
#include <cstdint>

namespace Chimera::Serial
{
  /*---------------------------------------------------------------------------
  Public Functions
  ---------------------------------------------------------------------------*/
  namespace Backend
  {
    /**
     *  Registers the backend driver with Chimera
     *
     *  @param[in]  registry    Chimera's copy of the driver interface
     *  @return Chimera::Status_t
     */
    extern Chimera::Status_t registerDriver( DriverConfig &registry );
  }  // namespace Backend

  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  /**
   * @brief Hardware interface to a typical serial port, aka UART
   */
  class HWInterface
  {
  public:
    virtual ~HWInterface() = default;

    /**
     * @brief Opens the port and readies it for operation
     *
     * @param config    Desired operating parameters
     * @return Chimera::Status_t
     */
    virtual Chimera::Status_t open( const Chimera::Serial::Config &config ) = 0;

    /**
     * @brief Closes the serial port
     * @return Chimera::Status_t
     */
    virtual Chimera::Status_t close() = 0;

    /**
     * @brief Writes data onto the wire
     *
     * @param buffer  Buffer to write
     * @param length  Number of bytes to write from the buffer
     * @param timeout Total time the transaction may take to occur in milliseconds
     * @return int    Number of bytes actually written, negative on error
     */
    virtual int write( const void *const buffer, const size_t length, const size_t timeout ) = 0;

    /**
     * @brief Read a number of bytes from the wire
     *
     * This will read from internal IO buffers.
     *
     * @param buffer  Buffer to read into
     * @param length  Number of bytes to read
     * @param timeout Total time the transaction may take to occur in milliseconds
     * @return int    Number of bytes actually read, negative on error
     */
    virtual int read( void *const buffer, const size_t length, const size_t timeout ) = 0;
  };

  /**
   *  Virtual class to facilitate easy mocking of the driver
   */
#if defined( CHIMERA_VIRTUAL )
  class ISerial : virtual public Chimera::Serial::HWInterface,
                  virtual public Chimera::Thread::AsyncIOInterface,
                  virtual public Chimera::Thread::LockableInterface
  {
  public:
    virtual ~ISerial() = default;
  };
#else
  class ISerial
  {
  };
#endif /* CHIMERA_VIRTUAL */

}  // namespace Chimera::Serial

#endif /* !CHIMERA_SERIAL_INTERFACE_HPP */
