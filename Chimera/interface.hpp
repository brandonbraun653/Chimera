/********************************************************************************
 *   File Name:
 *       interface.hpp
 *
 *   Description:
 *       Defines the hardware interface for the Chimera HAL. All libraries that
 *       depend upon Chimera are guaranteed to, at a bare minimum, have the
 *       behavior described here. IF that behavior is not implemented, that is
 *the fault of the underlying hardware drivers.
 *
 *   Note:
 *       This file is kept separate from the actual HAL include file for each HW
 *       driver to prevent possible recursive includes.
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/
#pragma once
#ifndef CHIMERA_INTERFACE_HPP
#define CHIMERA_INTERFACE_HPP

/* C++ Includes */
#include <array>
#include <cstdint>
#include <cstdlib>
#include <type_traits>

/* Boost Includes */
#include <boost/circular_buffer.hpp>

/* Chimera Includes */
#include <Chimera/config.hpp>
#include <Chimera/preprocessor.hpp>
#include <Chimera/threading.hpp>
#include <Chimera/types.hpp>

namespace Chimera
{
  namespace GPIO
  {
    /**
     * Defines expected behavior for all embedded systems that allow the user to control
     * GPIO pins. This is a pure virtual/abstract class.
     */
    class Interface : Threading::Lockable
    {
    public:
      /**
       *	Virtual destructor necessary for GMock as well as inheritors
       */
      virtual ~Interface() = default;

      /**
       *  Initialize the GPIO object to use a specific port and pin assignment.
       *
       *	@param[in]	port        The port to be used
       *	@param[in]	pin         The pin to be used
       *	@return Chimera::Status_t
       *
       *
       *  |   Return Value   |             Explanation            |
       *  |:----------------:|:----------------------------------:|
       *  |               OK | The GPIO was initialized correctly |
       *  |             FAIL | The GPIO failed initialization     |
       *  | INVAL_FUNC_PARAM | User passed in an invalid value    |
       */
      virtual Chimera::Status_t init( const Chimera::GPIO::Port port, const uint8_t pin ) = 0;

      /**
       *  Change the GPIO pin electrical input/output behavior mode
       *
       *	@param[in]	drive       The new drive mode to be set
       *	@param[in]	pullup      Enable or disable pullups
       *	@return Chimera::Status_t
       *
       *	|   Return Value   |                 Explanation                 |
       *  |:----------------:|:-------------------------------------------:|
       *  |               OK | The pin set the mode and pull-ups correctly |
       *  |             FAIL | The pin failed applying the settings        |
       *  | INVAL_FUNC_PARAM | User passed in an invalid value             |
       *  |  NOT_INITIALIZED | The GPIO object has not been initialized    |
       */
      virtual Chimera::Status_t setMode( const Chimera::GPIO::Drive drive, const bool pullup ) = 0;

      /**
       *  Change the logical state of the pin
       *
       *	@param[in]	state       The new state to transition into
       *	@return Chimera::Status_t
       *
       *	|   Return Value   |                 Explanation                 |
       *  |:----------------:|:-------------------------------------------:|
       *  |               OK | The pin set the new state correctly         |
       *  |             FAIL | The pin failed applying the settings        |
       *  |  NOT_INITIALIZED | The GPIO object has not been initialized    |
       */
      virtual Chimera::Status_t setState( const Chimera::GPIO::State state ) = 0;

      /**
       *  Read the current logical state of the pin
       *
       *	@param[out]	state       Variable to record the state info into
       *	@return Chimera::Status_t
       *
       *	|   Return Value  |                  Explanation                 |
       *  |:---------------:|:--------------------------------------------:|
       *  |              OK | The pin read the state correctly             |
       *  |            FAIL | The pin failed reading the state             |
       *  |   NOT_SUPPORTED | This behavior is not supported on the driver |
       *  | NOT_INITIALIZED | The GPIO object has not been initialized     |
       */
      virtual Chimera::Status_t getState( Chimera::GPIO::State &state )
      {
        /* Prevent GMOCK abstract instantiation error */
        return Chimera::GPIO::Status::NOT_SUPPORTED;
      }

      /**
       *  Toggle the state of the pin
       *
       *	@return Chimera::Status_t
       *
       *	|   Return Value  |                  Explanation                 |
       *  |:---------------:|:--------------------------------------------:|
       *  |              OK | The pin toggled correctly                    |
       *  |            FAIL | The pin failed toggling                      |
       *  |   NOT_SUPPORTED | This behavior is not supported on the driver |
       *  | NOT_INITIALIZED | The GPIO object has not been initialized     |
       */
      virtual Chimera::Status_t toggle() = 0;
    };

#ifndef CHIMERA_INHERITED_GPIO

    /**
     * Defines a real GPIO object that has completely disabled functionality. Used to allow code
     * to compile, but still fail at runtime in a clean manner.
     */
    class GPIOUnsupported : public Interface
    {
    public:
      GPIOUnsupported()  = default;
      ~GPIOUnsupported() = default;

      Chimera::Status_t init( const Chimera::GPIO::Port port, const uint8_t pin ) final override
      {
        return Chimera::GPIO::Status::FAIL;
      }

      Chimera::Status_t setMode( const Chimera::GPIO::Drive drive, const bool pullup ) final override
      {
        return Chimera::GPIO::Status::NOT_INITIALIZED;
      }

      Chimera::Status_t setState( const Chimera::GPIO::State state ) final override
      {
        return Chimera::GPIO::Status::NOT_INITIALIZED;
      }

      Chimera::Status_t getState( Chimera::GPIO::State &state ) final override
      {
        return Chimera::GPIO::Status::NOT_INITIALIZED;
      }

      Chimera::Status_t toggle() final override
      {
        return Chimera::GPIO::Status::NOT_INITIALIZED;
      }
    };

    using CHIMERA_INHERITED_GPIO = GPIOUnsupported;
#endif /* !CHIMERA_INHERITED_GPIO */

  }  // namespace GPIO

  namespace SPI
  {
    class Interface : public Threading::Lockable
    {
    public:
      virtual ~Interface() = default;

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
      virtual Chimera::Status_t init( const Chimera::SPI::Setup &setupStruct ) = 0;

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
      virtual Chimera::Status_t setChipSelect( const Chimera::GPIO::State &value ) = 0;

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
      virtual Chimera::Status_t setChipSelectControlMode( const Chimera::SPI::ChipSelectMode &mode ) = 0;

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
      virtual Chimera::Status_t writeBytes( const uint8_t *const txBuffer, const size_t length, const uint32_t timeoutMS ) = 0;

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
      virtual Chimera::Status_t readBytes( uint8_t *const rxBuffer, const size_t length, const uint32_t timeoutMS ) = 0;

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
      virtual Chimera::Status_t readWriteBytes( const uint8_t *const txBuffer, uint8_t *const rxBuffer, const size_t length,
                                                const uint32_t timeoutMS ) = 0;

      /**
       *  Set the hardware operational mode in either Blocking, Interrupt, or DMA.
       *
       *  @param[in]  periph          The peripheral to set the behavior on
       *  @param[in]  mode            Desired operational mode of the peripheral
       *  @return Chimera::Status_t
       *
       *  |   Return Value  |                  Explanation                 |
       *  |:---------------:|:--------------------------------------------:|
       *  |              OK | The operation completed successfully         |
       *  |            FAIL | The operation failed                         |
       *  | NOT_INITIALIZED | The class object has not been initialized    |
       */
      virtual Chimera::Status_t setPeripheralMode( const Chimera::SPI::SubPeripheral periph,
                                                   const Chimera::SPI::SubPeripheralMode mode ) = 0;

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
      virtual Chimera::Status_t setClockFrequency( const uint32_t freq, const uint32_t tolerance ) = 0;

      /**
       *  Get the current SPI clock frequency.
       *
       *  If the class has not been initialized, the output variable should not be modified.
       *
       *  @param[out] freq            Reported SPI clock
       *  @return Chimera::Status_t
       *
       *  |   Return Value  |                Explanation                |
       *  |:---------------:|:-----------------------------------------:|
       *  |              OK | The operation completed successfully      |
       *  |            FAIL | The operation failed                      |
       *  | NOT_INITIALIZED | The class object has not been initialized |
       */
      virtual Chimera::Status_t getClockFrequency( uint32_t &freq ) = 0;

      /**
       *  Reserves the SPI hardware to allow unobstructed use
       *
       *  @param[in]  timeout_ms      How many milliseconds to wait for the hardware to become available
       *  @return Chimera::Status_t
       *
       *  |  Return Value |                  Explanation                 |
       *  |:-------------:|:--------------------------------------------:|
       *  |            OK | The operation completed successfully         |
       *  |          FAIL | The operation failed                         |
       *  | NOT_SUPPORTED | This behavior is not supported on the driver |
       */
      virtual Chimera::Status_t reserve( const uint32_t timeout_mS ) override
      {
        return Chimera::SPI::Status::NOT_SUPPORTED;
      }

      /**
       *  Releases a previous reservation
       *
       *  @param[in]  timeout_ms      How many milliseconds to wait for the hardware to release
       *  @return Chimera::Status_t
       *
       *  |  Return Value |                  Explanation                 |
       *  |:-------------:|:--------------------------------------------:|
       *  |            OK | The operation completed successfully         |
       *  |          FAIL | The operation failed                         |
       *  | NOT_SUPPORTED | This behavior is not supported on the driver |
       */
      virtual Chimera::Status_t release( const uint32_t timeout_mS ) override
      {
        return Chimera::SPI::Status::NOT_SUPPORTED;
      }

      /**
       *  Allows the user to assign a callback function to the write complete event.
       *  The callback function will be passed a parameter indicating how many bytes were written.
       *
       *  @param[in]  func            User callback function
       *  @return Chimera::Status_t
       *
       *  |  Return Value |                  Explanation                 |
       *  |:-------------:|:--------------------------------------------:|
       *  |            OK | The operation completed successfully         |
       *  |          FAIL | The operation failed                         |
       *  | NOT_SUPPORTED | This behavior is not supported on the driver |
       */
      virtual Chimera::Status_t onWriteCompleteCallback( const Chimera::void_func_uint32_t func )
      {
        return Chimera::SPI::Status::NOT_SUPPORTED;
      }

      /**
       *  Allows the user to assign a callback function to the read complete event.
       *  The callback function will be passed a parameter indicating how many bytes were read.
       *
       *  @param[in]  func            User callback function
       *  @return Chimera::Status_t
       *
       *  |  Return Value |                  Explanation                 |
       *  |:-------------:|:--------------------------------------------:|
       *  |            OK | The operation completed successfully         |
       *  |          FAIL | The operation failed                         |
       *  | NOT_SUPPORTED | This behavior is not supported on the driver |
       */
      virtual Chimera::Status_t onReadCompleteCallback( const Chimera::void_func_uint32_t func )
      {
        return Chimera::SPI::Status::NOT_SUPPORTED;
      }

      /**
       *  Allows the user to assign a callback function to the read-write complete event.
       *  The callback function will be passed a parameter indicating how many bytes were read/written.
       *
       *  @param[in]  func            User callback function
       *  @return Chimera::Status_t
       *
       *  |  Return Value |                  Explanation                 |
       *  |:-------------:|:--------------------------------------------:|
       *  |            OK | The operation completed successfully         |
       *  |          FAIL | The operation failed                         |
       *  | NOT_SUPPORTED | This behavior is not supported on the driver |
       */
      virtual Chimera::Status_t onReadWriteCompleteCallback( const Chimera::void_func_uint32_t func )
      {
        return Chimera::SPI::Status::NOT_SUPPORTED;
      }

      /**
       *  Allows the user to assign a callback function on an error event
       *
       *  The function will be passed an error code indicating what happened
       *
       *  @param[in]  func            User callback function
       *  @return Chimera::Status_t
       *
       *  |  Return Value |                  Explanation                 |
       *  |:-------------:|:--------------------------------------------:|
       *  |            OK | The operation completed successfully         |
       *  |          FAIL | The operation failed                         |
       *  | NOT_SUPPORTED | This behavior is not supported on the driver |
       */
      virtual Chimera::Status_t onErrorCallback( const Chimera::void_func_uint32_t func )
      {
        return Chimera::SPI::Status::NOT_SUPPORTED;
      }
    };

#ifndef CHIMERA_INHERITED_SPI
    /**
     * Defines a real SPI object that has completely disabled functionality. Used to allow code
     * to compile, but still fail at runtime in a clean manner.
     */
    class SPIUnsupported : public Interface
    {
    public:
      SPIUnsupported()  = default;
      ~SPIUnsupported() = default;

      Chimera::Status_t init( const Chimera::SPI::Setup &setupStruct ) final override
      {
        return Chimera::SPI::Status::FAIL;
      }

      Chimera::Status_t deInit() final override
      {
        return Chimera::SPI::Status::FAIL;
      }

      Chimera::Status_t setChipSelect( const Chimera::GPIO::State &value ) final override
      {
        return Chimera::SPI::Status::NOT_SUPPORTED;
      }

      Chimera::Status_t setChipSelectControlMode( const Chimera::SPI::ChipSelectMode &mode ) final override
      {
        return Chimera::SPI::Status::NOT_INITIALIZED;
      }

      Chimera::Status_t writeBytes( const uint8_t *const txBuffer, size_t length, uint32_t timeoutMS ) final override
      {
        return Chimera::SPI::Status::FAIL;
      }

      Chimera::Status_t readBytes( uint8_t *const rxBuffer, size_t length, uint32_t timeoutMS ) final override
      {
        return Chimera::SPI::Status::FAIL;
      }

      Chimera::Status_t readWriteBytes( const uint8_t *const txBuffer, uint8_t *const rxBuffer, size_t length,
                                        uint32_t timeoutMS ) final override
      {
        return Chimera::SPI::Status::FAIL;
      }

      Chimera::Status_t setPeripheralMode( const Chimera::SPI::SubPeripheral periph,
                                           const Chimera::SPI::SubPeripheralMode mode ) final override
      {
        return Chimera::SPI::Status::FAIL;
      }

      Chimera::Status_t setClockFrequency( const uint32_t freq, const uint32_t tolerance ) final override
      {
        return Chimera::SPI::Status::FAIL;
      }

      Chimera::Status_t getClockFrequency( uint32_t &freq ) final override
      {
        return Chimera::SPI::Status::FAIL;
      }
    };

    using CHIMERA_INHERITED_SPI = SPIUnsupported;
#endif /* !CHIMERA_INHERITED_SPI */

  }  // namespace SPI

  namespace Serial
  {
    class Interface;
    using Interface_sPtr = std::shared_ptr<Interface>;
    using Interface_uPtr = std::unique_ptr<Interface>;

    class Interface : public Threading::Lockable
    {
    public:

      /**
       *  Attaches and configures the physical hardware channel and GPIO pin setup
       *
       *  @param[in]  channel     The physical UART hardware channel to use
       *  @param[in]  pins        TX and RX pin configuration
       */
      virtual Chimera::Status_t assignHW( const uint8_t channel, const IOPins &pins ) = 0;

      /**
       *   Starts up the Serial interface with a baud rate and transfer mode
       *
       *   @param[in]  txMode  What mode to run the TX hardware in
       *   @param[in]  rxMode  What mode to run the RX hardware in
       *   @return Chimera::Status_t
       */
      virtual Chimera::Status_t begin( const Modes txMode, const Modes rxMode ) = 0;

      /**
       *   De-initializes the serial port
       */
      virtual Chimera::Status_t end() = 0;

      /**
       *   Configures the serial port with the desired properties
       *
       */
      virtual Chimera::Status_t configure( const uint32_t baud, const CharWid width, const Parity parity, const StopBits stop,
                                           const FlowControl flow ) = 0;

      /**
       *   Change the baud rate of the peripheral at run time
       *
       *   @param[in]  baud    Desired baud rate to be used
       *   @return Chimera::Status_t
       */
      virtual Chimera::Status_t setBaud( const uint32_t baud ) = 0;

      /**
       *   Change the hardware transfer mode (Blocking, Interrupt, DMA)
       *
       *   @note When using Interrupt or DMA mode, double buffering must be enabled
       * first
       *   @see enableDoubleBuffering
       *
       *   @param[in]  periph  The peripheral to switch modes with
       *
       */
      virtual Chimera::Status_t setMode( const SubPeripheral periph, const Modes mode ) = 0;

      /**
       *   Writes data onto the wire
       *
       *   @note Depending on the mode, this function will behave a bit differently.
       *
       *   Blocking Mode:
       *       The function won't return until the data has been transmitted.
       *
       *   Interrupt & DMA:
       *       The function immediately returns after queuing up the transfer.
       * Double buffering must be enabled in order for these modes to work
       * correctly. Up to two transfers can be queued at once, the length being
       * limited to the size passed into enableDoubleBuffering().
       *
       *  @param[in]  buffer        The data to be written on the wire
       *  @param[in]  length        How many bytes to write
       *  @param[in]  timeout_mS    How long to wait on hardware before aborting
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t write( const uint8_t *const buffer, const size_t length,
                                       const uint32_t timeout_mS = 500 ) = 0;

      /**
       *   Read an exact number of bytes from the wire
       *
       *   @note Depending on the mode, this function will behave a bit differently.
       *
       *   Blocking Mode:
       *    The function won't return until the number of bytes specified has been received
       *    or the timeout has occurred.
       *
       *   Interrupt & DMA:
       *    The function immediately returns after queuing up the reception. The software can
       *    use the given buffer as a temporary storage location until the data is copied into
       *    the circular buffer that was passed in via enableBuffering(). The user can then be
       *    notified of completion by either an event notifier or by checking the available()
       *    function.
       *
       *  @see enableBuffering
       *  @see attachEventNotifier
       *
       *  @param[in]  buffer        In blocking mode, will contain data read from RX
       *  @param[in]  length        How many bytes to read
       *  @param[in]  timeout_mS    How long to wait on hardware before aborting
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t read( uint8_t *const buffer, const size_t length,
                                      const uint32_t timeout_mS = 500 ) = 0;

      /**
       *	Flushes out the given subperipheral queues
       *
       *	@param[in]	periph        The peripheral to be flushed
       *	@return Chimera::Status_t
       */
       virtual Chimera::Status_t flush( const SubPeripheral periph ) = 0;

      /**
       *  Read data queued from the RX buffer. This buffer can only be filled if the
       *  hardware was placed in Interrupt or DMA RX mode.
       *
       *  @param[in]  buffer  Array to store the data into
       *  @param[in]  len     The number of bytes to read from the RX buffer
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t readAsync( uint8_t *const buffer, const size_t len )
      {
        return Status::NOT_SUPPORTED;
      }

      /**
       *  Turns on buffering for asynchronous modes (Interrupt, DMA)
       *
       *  Allows the Serial channel to read/write data on one buffer while the user
       *  can read/write on the other. This should help prevent missing data when the
       *  RX length is unknown or when there is lots of traffic.
       *
       *   @note Either buffer could be modified inside an ISR, hence the necessity for volatile storage class.
       *
       *   @param[in]  periph      The peripheral (TX or RX) to buffer on
       *   @param[in]  buffer      The buffer to use
       *   @param[in]  length      The length of the buffer
       *   @return Chimera::Status_t
       */
      virtual Chimera::Status_t enableBuffering( const SubPeripheral periph, boost::circular_buffer<uint8_t> *const buffer )
      {
        return Status::NOT_SUPPORTED;
      }

      /**
       *  Turns off the buffering feature
       *
       *  @note This will automatically transition both TX & RX sub-peripherals back
       *   to blocking mode
       *
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t disableBuffering(const SubPeripheral periph)
      {
        return Status::NOT_SUPPORTED;
      }

      /**
       *   Check if data is available to be read. Only works when double buffering
       * is enabled.
       *
       *   @param[in]  bytes   Optionally report back how many bytes are ready
       *   @return True if any data is ready, false if not
       */
      virtual bool available( size_t *const bytes = nullptr )
      {
        return false;
      }

      /**
       *   Attach a signal to get notified when an event occurs
       *
       *   @param[in]  event       The event to be notified on
       *   @param[in]  notifier    The notification variable
       *   @return void
       */
      virtual Chimera::Status_t attachEventNotifier( const Event event, volatile bool *const notifier )
      {
        return Status::NOT_SUPPORTED;
      }

      /**
       *   Remove an event notification signal
       *
       *   @param[in]  event       The event to remove the notifier
       *   @param[in]  notifier    The notification variable
       *   @return void
       */
      virtual Chimera::Status_t removeEventNotifier( const Event event, volatile bool *const notifier )
      {
        return Status::NOT_SUPPORTED;
      }

#if defined( USING_FREERTOS )
      /**
       *   Attach a signal to get notified when an event occurs
       *
       *   @param[in]  event   The event to be notified on
       *   @param[in]  semphr  The notification variable
       *   @return void
       */
      virtual Chimera::Status_t attachEventNotifier( const Event event, SemaphoreHandle_t *const semphr )
      {
        return Status::NOT_SUPPORTED;
      }

      /**
       *   Remove an event notification signal
       *
       *   @param[in]  event   The event to remove the notifier
       *   @param[in]  semphr  The notification variable
       *   @return void
       */
      virtual Chimera::Status_t removeEventNotifier( const Event event, SemaphoreHandle_t *const semphr )
      {
        return Status::NOT_SUPPORTED;
      }
#endif

      virtual Chimera::Status_t reserve( const uint32_t timeout_mS ) override
      {
        return false;
      }

      virtual Chimera::Status_t release( const uint32_t timeout_mS ) override
      {
        return false;
      }
    };

#ifndef CHIMERA_INHERITED_SERIAL

    class SerialUnsupported : public Interface
    {

      //TODO: Do this once you have access to VAssist again...

    };

    using CHIMERA_INHERITED_SERIAL = SerialUnsupported;
#endif

  }  // namespace Serial

  namespace System
  {
    class Interface
    {
    public:
      virtual Status reasonForReset( ResetType &reason )
      {
        reason = ResetType::UNKNOWN_RESET;
        return Status::NOT_SUPPORTED;
      }

    private:
    };

#if ( CHIMERA_HWM_SYSCTL == 0 )
    typedef Interface CHIMERA_INHERITED_SYSCTL;
#endif

  }  // namespace System

  namespace Watchdog
  {
    class Interface
    {
    public:
      /**
       *   Initializes the low level hardware needed to configure the watchdog
       * peripheral. This does not start the timer.
       *
       *   @note   Guarantees a minimum resolution of +/- 500uS around the specified
       * timeout
       *
       *   @param[in]  timeout_mS      How many milliseconds can elapse before
       * watchdog expires
       *   @return Status::OK if the initialization was a success, Status::FAIL if
       * not
       */
      virtual Status initialize( const uint32_t timeout_mS )
      {
        return Status::NOT_SUPPORTED;
      }

      /**
       *   Starts the watchdog timer. If successful, Interface::kick() must
       *   be called at regular intervals to prevent the watchdog from firing.
       *
       *   @return Peripheral status
       */
      virtual Status start()
      {
        return Status::NOT_SUPPORTED;
      }

      /**
       *   Stops the watchdog timer.
       *
       *   @return Peripheral status
       */
      virtual Status stop()
      {
        return Status::NOT_SUPPORTED;
      }

      /**
       *   Kicks the watchdog timer, starting a new countdown cycle.
       *
       *   @return Peripheral status
       */
      virtual Status kick()
      {
        return Status::NOT_SUPPORTED;
      }

      /**
       *   Gets the actual timeout value achieved by the hardware
       *
       *   @param[out] timeout     Timeout value in milliseconds
       *   @return Peripheral status
       */
      virtual Status getTimeout( uint32_t &timeout )
      {
        timeout = 0u;
        return Status::NOT_SUPPORTED;
      }

      /**
       *   Configures the watchdog to stop on connection to a debugger
       *
       *   @param[in]  enable      If true, allows the watchdog to stop. Otherwise,
       * it continues running
       *   @return Peripheral status
       */
      virtual Status pauseOnDebugHalt( const bool enable )
      {
        return Status::NOT_SUPPORTED;
      }

      virtual bool isSupported()
      {
        return false;
      }

      virtual ~Interface() = default;
    };

  }  // namespace Watchdog
}  // namespace Chimera

#endif /* !CHIMERA_INTERFACE_HPP */
