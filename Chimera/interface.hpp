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

/* Chimera Includes */
#include <Chimera/config.hpp>
#include <Chimera/preprocessor.hpp>
#include <Chimera/threading.hpp>
#include <Chimera/types.hpp>

namespace Chimera
{
  namespace GPIO
  {
    class Interface : Threading::Lockable
    {
    public:
      virtual Chimera::GPIO::Status init( const Chimera::GPIO::Port port, const uint8_t pin ) = 0;

      virtual Chimera::GPIO::Status setMode( const Chimera::GPIO::Drive drive, const bool pullup ) = 0;

      virtual Chimera::GPIO::Status setState( const Chimera::GPIO::State state ) = 0;

      virtual Chimera::GPIO::Status getState( Chimera::GPIO::State &state ) = 0;

      virtual Chimera::GPIO::Status toggle() = 0;

      virtual ~Interface() = default;
    };
  }  // namespace GPIO

  namespace SPI
  {
    class Interface : public Threading::Lockable
    {
    public:
 
      virtual ~Interface() = default;

      virtual void testfunc() {};

      /**
       *  @brief Initializes the SPI hardware according to the setup struct
       *
       *  @param[in]   setupStruct     Contains information on how to initialize SPI
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t init( const Chimera::SPI::Setup &setupStruct ) = 0;

      /**
       *	@brief Destroys all previous hardware setup (virtually or physically)
       *	Usage of the object after this call requires re-initialization.
       *
       *	@return Chimera::Status_t
       */
       virtual Chimera::Status_t deInit() = 0;

      /**
       *  @brief Sets the chip select GPIO to a logical state
       *
       *  @param[in]   value   The state to set the chip select to
       */
      virtual Chimera::Status_t setChipSelect( const Chimera::GPIO::State &value ) = 0;

      /**
       *  @brief Instruct the chip select to behave in a specific manner
       *
       *  @param[in]  mode    The desired mode for the chip select to operate in
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t setChipSelectControlMode( const Chimera::SPI::ChipSelectMode &mode ) = 0;

      /**
       *  @brief Writes data onto the SPI bus
       *
       *  @param[in]  txBuffer    Data buffer to be sent
       *  @param[in]  length      Number of bytes to be sent
       *  @param[in]  disableCS   Optionally disable the chip select line after
       * transmission complete
       *  @param[in]  autoRelease Optionally release the SPI HW lock should the
       * caller hold ownership
       *  @param[in]  timeoutMS   If the hardware is not free, wait this amount of
       * time before exiting
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t writeBytes( const uint8_t *const txBuffer, size_t length, const bool &disableCS = true,
                                               const bool &autoRelease = false, uint32_t timeoutMS = 10 ) = 0;

      /**
       *  @brief Writes data on to the SPI bus
       *  Acts as a conventient mapping tool to allow the use of std::array to write
       * data
       *
       *  @param[in]  txBuffer    Data buffer to be sent
       *  @param[in]  disableCS   Optionally disable the chip select line after
       * transmission complete
       *  @param[in]  autoRelease Optionally release the SPI HW lock should the
       * caller hold ownership
       *  @param[in]  timeoutMS   If the hardware is not free, wait this amount of
       * time before exiting
       *  @return Chimera::Status_t
       */
      template<typename T, std::size_t S>
      Chimera::Status_t writeBytes( const std::array<T, S> &txBuffer, const bool &disableCS = true,
                                       const bool &autoRelease = false, uint32_t timeoutMS = 10 )
      {
        auto constexpr arr = static_cast<uint8_t *>( txBuffer.data() );
        return writeBytes( arr, txBuffer.size(), disableCS, autoRelease, timeoutMS );
      }

      /**
       *  @brief Reads data from the SPI bus
       *
       *  Internally,the function will call readWriteBytes() and use the rxBuffer as
       * the source of bytes to transmit on the MOSI line.
       *
       *  Master
       *  ==================
       *  This section describes the behavior of readBytes() when SPI is configured
       * as a master.
       *
       *      Blocking Mode
       *      ------------------
       *      When called in blocking mode, the SPI hardware will attempt to read a
       * number of bytes from the bus with an enforced timeout. Should the requested
       * number of bytes not arrive before the timeout expires, an error code is
       * returned indicating the timeout.
       *
       *      Interrupt Mode
       *      ------------------
       *      When using interrupt mode, an ISR handles the transfer. If supported
       * by the underlying driver, the user can attach callbacks to be notified when
       * the transfer is complete [onReadCompleteCallback()] or when an error occurs
       * [onErrorCallback()].
       *
       *      DMA Mode
       *      ------------------
       *      The DMA hardware will handle the transfer. If supported by the
       * underlying driver, the user can attach callbacks to be notified when the
       * transfer is complete [onReadCompleteCallback()] or when an error occurs
       * [onErrorCallback()].
       *
       *
       *  Slave
       *  ==================
       *  This section describes the behavior of readBytes() when SPI is configured
       * as a slave.
       *
       *      Blocking Mode
       *      ------------------
       *      When called in blocking mode, the SPI hardware will attempt to read a
       * number of bytes from MOSI with an enforced timeout. The corresponding bytes
       * clocked out on MISO will be any data contained in the receive buffer before
       * the transfer starts. Should the requested number of bytes not arrive before
       * the timeout expires or the slave select line is not asserted, an error code
       * is returned indicating the timeout.
       *
       *      Interrupt Mode
       *      ------------------
       *
       *
       *      DMA Mode
       *      ------------------
       *
       *
       *  @param[out]  rxBuffer    Data buffer to read into
       *  @param[in]   length      Number of bytes to read
       *  @param[in]   disableCS   Optionally disable the chip select line after
       * transmission complete
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t readBytes( uint8_t *const rxBuffer, size_t length, const bool &disableCS = true,
                                              const bool &autoRelease = false, uint32_t timeoutMS = 10 ) = 0;

      /**
       *	@brief A templated version of readBytes() for more modern C++
       *compatibility
       *	@see readBytes()
       */
      template<typename T, std::size_t S>
      Chimera::Status_t readBytes( std::array<T, S> rxBuffer, const bool &disableCS = true, const bool &autoRelease = false,
                                      uint32_t timeoutMS = 10 )
      {
        auto constexpr array = static_cast<uint8_t *>( rxBuffer.data() );
        return readBytes( array, rxBuffer.size(), disableCS, autoRelease, timeoutMS );
      }

      /**
       *  @brief Transmits and receives data on the SPI bus
       *
       *  @param[in]   txBuffer    Data buffer to write from
       *  @param[out]  rxBuffer    Data buffer to read into
       *  @param[in]   length      Number of bytes to transfer
       *  @param[in]   disableCS   Optionally disable the chip select line after
       * transmission complete
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t readWriteBytes( const uint8_t *const txBuffer, uint8_t *const rxBuffer, size_t length,
                                                   const bool &disableCS = true, const bool &autoRelease = false,
                                                   uint32_t timeoutMS = 10 ) = 0;

      /**
       *  @brief Set the hardware peripheral operational mode.
       *  This should allow the user to choose if the hardware operates in blocking,
       * interrupt, or DMA mode
       *
       *  @param[in]   periph      The peripheral to set the behavior on
       *  @param[in]   mode        Desired operational mode of the peripheral
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t setPeripheralMode( const Chimera::SPI::SubPeripheral &periph,
                                                      const Chimera::SPI::SubPeripheralMode &mode ) = 0;

      /**
       *  @brief Change the frequency of the SPI output clock
       *
       *  Should work at runtime after the SPI hardware has been configured. If the
       *  exact clock frequency cannot be met, the next lowest value will be selected
       *  up to the hardware limits. 
       *
       *  For example, if a particular device supports 1MHz, 2MHz, 4MHz and 8MHz clock rates
       *  and the user requests a clock of 7.5MHz, the hardware will be initialized to 4MHz and
       *  return a status of Chimera::SPI::Status::CLOCK_SET_LT
       *
       *  @param[in]   freq    Desired SPI clock frequency in Hz
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t setClockFrequency( const uint32_t &freq ) = 0;

      /**
       *  @brief Get the current SPI clock frequency
       *
       *  @param[out]  freq    Reported SPI clock
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t getClockFrequency( uint32_t *const freq ) = 0;

      /**
       *  @brief Reserves the SPI hardware to allow unobstructed use
       *
       *  @param[in]  timeout_ms  How many milliseconds to wait for the hardware to
       * become available
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t reserve( const uint32_t timeout_mS ) override
      {
        return Chimera::SPI::Status::NOT_SUPPORTED;
      }

      /**
       *  @brief Releases a previous reservation
       *
       *  @param[in]  timeout_ms  How many milliseconds to wait for the hardware to
       * release
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t release( const uint32_t timeout_mS ) override
      {
        return Chimera::SPI::Status::NOT_SUPPORTED;
      }

      /**
       *  @brief Allows the user to assign a callback function to the write complete
       * event
       *
       *  @param[in]  func  Callback function
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t onWriteCompleteCallback( const Chimera::void_func_void func )
      {
        return Chimera::SPI::Status::NOT_SUPPORTED;
      }

      /**
       *  @brief Allows the user to assign a callback function to the read complete
       * event
       *
       *  @param[in]  func  Callback function
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t onReadCompleteCallback( const Chimera::void_func_void func )
      {
        return Chimera::SPI::Status::NOT_SUPPORTED;
      }

      /**
       *  @brief Allows the user to assign a callback function to the read-write
       * complete event
       *
       *  @param[in]  func  Callback function
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t onReadWriteCompleteCallback( const Chimera::void_func_void func )
      {
        return Chimera::SPI::Status::NOT_SUPPORTED;
      }

      /**
       *  @brief Allows the user to assign a callback function on an error event
       *
       *  The function will be passed an error code indicating what happened
       *
       *  @param[in]  func  Callback function
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t onErrorCallback( const Chimera::void_func_uint32_t func )
      {
        return Chimera::SPI::Status::NOT_SUPPORTED;
      }

#if defined( USING_FREERTOS )

      /**
       *  @brief Allows the user to have a semaphore given to when an event occurs
       *
       *  @param[in]  event   The event to be waiting on
       *  @param[in]  semphr  The semaphore to be given to upon event occurrence
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t attachEventWakeup( const Chimera::FreeRTOS::SPIEvent &event,
                                                      const SemaphoreHandle_t *const semphr ) = 0;

      /**
       *  @brief Removes a semaphore from the event wakeup list
       *
       *  @param[in]  semphr  The semaphore to be removed
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t removeEventWakeup( const SemaphoreHandle_t *const semphr ) = 0;

#endif /* !USING_FREERTOS */

    };
  }  // namespace SPI

  namespace Serial
  {
    class Interface : public Threading::Lockable
    {
    public:
      /**
       *   Starts up the Serial interface with a baud rate and transfer mode
       *
       *   @param[in]  txMode  What mode to run the TX hardware in
       *   @param[in]  rxMode  What mode to run the RX hardware in
       *   @return Chimera::Status_t
       */
      virtual Chimera::Status_t begin( const Modes txMode, const Modes rxMode ) noexcept = 0;

      /**
       *   De-initializes the serial port
       */
      virtual Chimera::Status_t end() noexcept = 0;

	    /**
       *   Configures the serial port with the desired properties
       *
       */
      virtual Chimera::Status_t configure( const uint32_t baud, const CharWid width, const Parity parity,
                                                 const StopBits stop, const FlowControl flow ) noexcept = 0;

      /**
       *   Change the baud rate of the peripheral at run time
       *
       *   @param[in]  baud    Desired baud rate to be used
       *   @return Chimera::Status_t
       */
      virtual Chimera::Status_t setBaud( const uint32_t buad ) noexcept = 0;

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
      virtual Chimera::Status_t setMode( const SubPeripheral periph, const Modes mode ) noexcept = 0;

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
                                             const uint32_t timeout_mS = 500 ) noexcept = 0;

      /**
       *   Read an exact number of bytes from the wire
       *
       *   @note Depending on the mode, this function will behave a bit differently.
       *
       *   Blocking Mode:
       *       The function won't return until the number of bytes specified has
       * been received.
       *
       *   Interrupt & DMA:
       *       The function immediately returns after queuing up the reception.
       * Double buffering must be enabled in order for these modes to work
       * correctly. Up to two receptions can be queued at once, the length being
       * limited to the size passed into enableDoubleBuffering().
       *
       *  @param[in]  buffer        The data to be received from the wire
       *  @param[in]  length        How many bytes to read
       *  @param[in]  timeout_mS    How long to wait on hardware before aborting
       *  @return Chimera::Status_t
       */
      virtual Chimera::Status_t read( uint8_t *const buffer, const size_t length,
                                            const uint32_t timeout_mS = 500 ) noexcept = 0;

      /**
       *   Read bytes from the wire, but the length to read is unknown.
       *
       *   @param[in]  buffer  Array to store the data into
       *   @param[in]  maxLen  Max number of bytes that can be read into the array
       *   @return Chimera::Status_t
       */
      virtual Chimera::Status_t readAsync( uint8_t *const buffer, const size_t maxLen ) noexcept
      {
        return Status::NOT_SUPPORTED;
      }

      /**
       *   Get the state of common hardware registers and status fields
       *
       *   @param[in]  status  Structure to fill with status information
       *   @return void
       */
      virtual void status( HardwareStatus &status ) noexcept
      {
      }

      /**
       *   Turns on double buffering for asynchronous modes (Interrupt, DMA)
       *
       *   Allows the Serial channel to read/write data on one buffer while the user
       * can read/write on the other. This should help prevent missing data when the
       * RX length is unknown or when there is lots of traffic.
       *
       *   @note Either buffer could be modified inside an ISR, hence the necessity
       * for volatile storage class.
       *
       *   @param[in]  periph      The peripheral (TX or RX) to use double buffering
       * with
       *   @param[in]  bufferOne   First buffer used
       *   @param[in]  bufferTwo   Second buffer used
       *   @param[in]  length      The minimum size of both buffers
       *   @return Chimera::Status_t
       */
      virtual Chimera::Status_t enableDoubleBuffering( const SubPeripheral periph, volatile uint8_t *const bufferOne,
                                                             volatile uint8_t *const bufferTwo, const size_t length ) noexcept
      {
        return Status::NOT_SUPPORTED;
      }

      /**
       *   Turns off the double buffering feature
       *
       *   @note This will automatically transition both TX & RX sub-peripherals back
       *    to blocking mode
       *
       *   @return Chimera::Status_t
       */
      virtual Chimera::Status_t disableDoubleBuffering() noexcept
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
      virtual bool available( size_t *const bytes = nullptr ) noexcept
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
      virtual Chimera::Status_t attachEventNotifier( const Event event, volatile bool *const notifier ) noexcept
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
      virtual Chimera::Status_t removeEventNotifier( const Event event, volatile bool *const notifier ) noexcept
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
      virtual Chimera::Status_t attachEventNotifier( const Event event, SemaphoreHandle_t *const semphr ) noexcept
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
      virtual Chimera::Status_t removeEventNotifier( const Event event, SemaphoreHandle_t *const semphr ) noexcept
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
    typedef Interface CHIMERA_INHERITED_SERIAL;
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
