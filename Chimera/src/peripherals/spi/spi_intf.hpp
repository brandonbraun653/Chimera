/********************************************************************************
 *  File Name:
 *    spi_intf.hpp
 *
 *  Description:
 *    Models the Chimera SPI interface
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SPI_INTERFACE_HPP
#define CHIMERA_SPI_INTERFACE_HPP

/* STL Includes */
#include <cstdint>

/* Chimera Includes */
#include <Chimera/buffer>
#include <Chimera/common>
#include <Chimera/event>
#include <Chimera/thread>

#include <Chimera/src/peripherals/spi/spi_types.hpp>

namespace Chimera::SPI
{
  namespace Backend
  {
    extern void prjInitialize();
  }

  class HardwareDriverInterface
  {
  public:
    virtual ~HardwareDriverInterface() = default;

    /**
     *  Initializes the SPI hardware according to the setup struct
     *
     *  @param[in]  setupStruct     Contains information on how to initialize SPI
     *  @return Chimera::Status_t
     *
     *  |   Return Value   |                        Explanation                        |
     *  |:----------------:|:---------------------------------------------------------:|
     *  |               OK | The operation completed successfully                      |
     *  |             FAIL | The operation failed                                      |
     *  | INVAL_FUNC_PARAM | One or more of the initialization parameters were invalid |
     */
    virtual Chimera::Status_t init( const Chimera::SPI::DriverConfig &setupStruct ) = 0;

    /**
     *  Gets the parameters used to initialize the SPI object
     *
     *  @return Chimera::SPI::DriverConfig
     */
    virtual Chimera::SPI::DriverConfig getInit() = 0;

    /**
     *	Destroys all previous hardware setup (virtually or physically), which requires
     *	re-initialization of the object in order to be used again.
     *
     *	@return Chimera::Status_t
     *
     *	| Return Value |              Explanation             |
     *  |:------------:|:------------------------------------:|
     *  |           OK | The operation completed successfully |
     *  |         FAIL | The operation failed                 |
     */
    virtual Chimera::Status_t deInit() = 0;

    /**
     *  Sets the chip select GPIO to a logical state
     *
     *  @param[in]  value           The state to set the chip select to
     *  @return Chimera::Status_t
     *
     *  |   Return Value  |                  Explanation                 |
     *  |:---------------:|:--------------------------------------------:|
     *  |              OK | The operation completed successfully         |
     *  |            FAIL | The operation failed                         |
     *  |   NOT_SUPPORTED | This behavior is not supported on the driver |
     *  | NOT_INITIALIZED | The class object has not been initialized    |
     */
    virtual Chimera::Status_t setChipSelect( const Chimera::GPIO::State value ) = 0;

    /**
     *  Instruct the chip select to behave in a specific manner
     *
     *  @param[in]  mode            The desired mode for the chip select to operate in
     *  @return Chimera::Status_t
     *
     *  |   Return Value  |                  Explanation                 |
     *  |:---------------:|:--------------------------------------------:|
     *  |              OK | The operation completed successfully         |
     *  | NOT_INITIALIZED | The class object has not been initialized    |
     */
    virtual Chimera::Status_t setChipSelectControlMode( const Chimera::SPI::CSMode mode ) = 0;

    /**
     *  Writes data onto the SPI bus. The number of bytes actually written will be returned
     *  via onWriteCompleteCallback().
     *
     *  @param[in]  txBuffer        Data buffer to be sent
     *  @param[in]  length          Number of bytes to be sent (should not be larger than txBuffer)
     *  @param[in]  timeoutMS       How long to wait for SPI hardware to become available
     *  @return Chimera::Status_t
     *
     *  |   Return Value  |                  Explanation                 |
     *  |:---------------:|:--------------------------------------------:|
     *  |              OK | The operation completed successfully         |
     *  |            FAIL | The operation failed                         |
     *  | NOT_INITIALIZED | The class object has not been initialized    |
     */
    virtual Chimera::Status_t writeBytes( const void *const txBuffer, const size_t length, const size_t timeoutMS ) = 0;

    /**
     *  Reads data from the SPI bus. The number of bytes actually read will be returned
     *  via onReadCompleteCallback().
     *
     *  @param[out] rxBuffer        Data buffer to read into
     *  @param[in]  length          Number of bytes to read (must not be larger than rxBuffer size)
     *  @param[in]  timeoutMS       How long to wait for SPI hardware to become available
     *  @return Chimera::Status_t
     *
     *  |   Return Value  |                  Explanation                 |
     *  |:---------------:|:--------------------------------------------:|
     *  |              OK | The operation completed successfully         |
     *  |            FAIL | The operation failed                         |
     *  | NOT_INITIALIZED | The class object has not been initialized    |
     */
    virtual Chimera::Status_t readBytes( void *const rxBuffer, const size_t length, const size_t timeoutMS ) = 0;

    /**
     *  Transmits and receives data on the SPI bus in a single operation. Returns the actual
     *  number of bytes transmitted/received via onReadWriteCompleteCallback().
     *
     *  @param[in]  txBuffer        Data buffer to write from
     *  @param[out] rxBuffer        Data buffer to read into
     *  @param[in]  length          Number of bytes to transfer (must not be larger than rxBuffer size)
     *  @param[in]  timeoutMS       How long to wait for SPI hardware to become available
     *  @return Chimera::Status_t
     *
     *  |   Return Value  |                  Explanation                 |
     *  |:---------------:|:--------------------------------------------:|
     *  |              OK | The operation completed successfully         |
     *  |            FAIL | The operation failed                         |
     *  | NOT_INITIALIZED | The class object has not been initialized    |
     */
    virtual Chimera::Status_t readWriteBytes( const void *const txBuffer, void *const rxBuffer, const size_t length,
                                              const size_t timeoutMS ) = 0;

    /**
     *  Set the hardware operational mode in either Blocking, Interrupt, or DMA.
     *
     *  @param[in]  mode            Desired operational mode of the peripheral
     *  @return Chimera::Status_t
     *
     *  |   Return Value  |                  Explanation                 |
     *  |:---------------:|:--------------------------------------------:|
     *  |              OK | The operation completed successfully         |
     *  |            FAIL | The operation failed                         |
     *  | NOT_INITIALIZED | The class object has not been initialized    |
     */
    virtual Chimera::Status_t setPeripheralMode( const Chimera::Hardware::PeripheralMode mode ) = 0;

    /**
     *  Change the frequency of the SPI output clock
     *
     *  Should work at runtime after the SPI hardware has been configured. If the
     *  exact clock frequency cannot be met, the next lowest value will be selected
     *  up to the hardware limits.
     *
     *  For example, if a particular device supports 1MHz, 2MHz, 4MHz and 8MHz clock rates
     *  and the user requests a clock of 7.5MHz with 0% tolerance, the hardware will be
     *  initialized to 4MHz and return a status of Chimera::SPI::Status::CLOCK_SET_LT.
     *
     *  @param[in]  freq            Desired SPI clock frequency in Hz
     *  @param[in]  tolerance       Percent tolerance allowed: 0 for exact, 100 for no care
     *  @return Chimera::Status_t
     *
     *  |   Return Value  |                              Explanation                             |
     *  |:---------------:|:--------------------------------------------------------------------:|
     *  |              OK | The operation completed successfully (CLOCK_SET_EQ)                  |
     *  |            FAIL | The operation failed                                                 |
     *  |   NOT_SUPPORTED | This behavior is not supported on the driver                         |
     *  | NOT_INITIALIZED | The class object has not been initialized                            |
     *  |    CLOCK_SET_EQ | The desired clock value was achieved exactly or within tolerance     |
     *  |    CLOCK_SET_LT | The actual clock value achieved was less than the user desired value |
     */
    virtual Chimera::Status_t setClockFrequency( const size_t freq, const size_t tolerance ) = 0;

    /**
     *  Get the current SPI clock frequency in Hz
     *
     *  @return size_t
     */
    virtual size_t getClockFrequency() = 0;
  };

  class ISPI : virtual public HardwareDriverInterface,
               virtual public Chimera::Event::ListenerInterface,
               virtual public Chimera::Threading::AsyncIOInterface,
               virtual public Chimera::Threading::LockableInterface
  {
  public:
    virtual ~ISPI() = default;
  };
}  // namespace Chimera::SPI

#endif /* !CHIMERA_SPI_INTERFACE_HPP */