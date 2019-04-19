/********************************************************************************
 *   File Name:
 *    serial_intf.hpp
 *
 *   Description:
 *    Models the Chimera Serial interface 
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once 
#ifndef CHIMERA_SERIAL_INTERFACE_HPP
#define CHIMERA_SERIAL_INTERFACE_HPP

/* C++ Includes */
#include <cstdint>

/* Chimera Includes */
#include <Chimera/preprocessor.hpp>
#include <Chimera/threading.hpp>
#include <Chimera/types/common_types.hpp>
#include <Chimera/types/serial_types.hpp>

/* Boost Includes */
#include <boost/circular_buffer_fwd.hpp>

namespace Chimera
{
  namespace Serial
  {
    class Interface : public Threading::Lockable
    {
    public:
      virtual ~Interface() = default;

      /**
       *  Attaches and configures the physical hardware channel and GPIO pin setup
       *
       *  @param[in]  channel       The physical UART hardware channel to use
       *  @param[in]  pins          TX and RX pin configuration
       *  @return Chimera::Status_t
       *
       *  | Return Value |          Explanation          |
       *  |:------------:|:-----------------------------:|
       *  |           OK | Everything worked as expected |
       *  |         FAIL | The function failed           |
       */
      virtual Chimera::Status_t assignHW( const uint8_t channel, const IOPins &pins ) = 0;

      /**
       *  Starts up the Serial interface with a baud rate and transfer mode
       *
       *  @param[in]  txMode        What mode to run the TX hardware in
       *  @param[in]  rxMode        What mode to run the RX hardware in
       *  @return Chimera::Status_t
       *
       *  | Return Value |          Explanation          |
       *  |:------------:|:-----------------------------:|
       *  |           OK | Everything worked as expected |
       *  |         FAIL | The function failed           |
       */
      virtual Chimera::Status_t begin( const Modes txMode, const Modes rxMode ) = 0;

      /**
       *  De-initializes the serial port
       *
       *  @return Chimera::Status_t
       *
       *  | Return Value |          Explanation          |
       *  |:------------:|:-----------------------------:|
       *  |           OK | Everything worked as expected |
       *  |         FAIL | The function failed           |
       */
      virtual Chimera::Status_t end() = 0;

      /**
       *  Configures the serial port with the desired properties
       *
       *  @param[in]  baud          The serial baud rate
       *  @param[in]  width         The number of bits per transfer
       *  @param[in]  parity        Parity options
       *  @param[in]  stop          Selects 1, 1.5, or 2 stop bits
       *  @param[in]  flow          Selects whether or not to use flow control
       *  @return Chimera::Status_t
       *
       *  |   Return Value   |                     Explanation                    |
       *  |:----------------:|:--------------------------------------------------:|
       *  |               OK | Everything worked as expected                      |
       *  |             FAIL | The function failed configuration of the hardware  |
       *  |  NOT_INITIALIZED | The serial hardware is not ready for configuration |
       *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function      |
       */
      virtual Chimera::Status_t configure( const uint32_t baud, const CharWid width, const Parity parity, const StopBits stop,
                                           const FlowControl flow ) = 0;

      /**
       *  Change the baud rate of the peripheral at run time
       *
       *  @param[in]  baud          Desired baud rate to be used
       *  @return Chimera::Status_t
       *
       *  |   Return Value   |                     Explanation                    |
       *  |:----------------:|:--------------------------------------------------:|
       *  |               OK | Everything worked as expected                      |
       *  |             FAIL | The function failed configuration of the hardware  |
       *  |  NOT_INITIALIZED | The serial hardware is not ready for configuration |
       */
      virtual Chimera::Status_t setBaud( const uint32_t baud ) = 0;

      /**
       *  Change the hardware transfer mode (Blocking, Interrupt, DMA)
       *
       *  @note When using Interrupt or DMA mode, buffering must be enabled first
       *  @see enableBuffering
       *
       *  @param[in]  periph        The peripheral to switch modes with
       *  @param[in]  mode          What the new mode should be
       *  @return Chimera::Status_t
       *
       *  |   Return Value   |                     Explanation                    |
       *  |:----------------:|:--------------------------------------------------:|
       *  |               OK | Everything worked as expected                      |
       *  |             FAIL | The function failed configuration of the hardware  |
       *  |  NOT_INITIALIZED | The serial hardware is not ready for configuration |
       *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function      |
       */
      virtual Chimera::Status_t setMode( const SubPeripheral periph, const Modes mode ) = 0;

      /**
       *  Writes data onto the wire
       *
       *  @note Depending on the mode, this function will behave a bit differently.
       *
       *  #### Blocking Mode
       *    The function won't return until the data has been transmitted.
       *
       *  #### Interrupt & DMA
       *    The function immediately returns after queuing up the transfer. Buffering must first be enabled
       *    using enableBuffering(). Notification of write complete can be achieved through an event notifier.
       *
       *  @see enableBuffering
       *  @see attachEventNotifier
       *
       *  @param[in]  buffer        The data to be written on the wire
       *  @param[in]  length        How many bytes to write
       *  @param[in]  timeout_mS    How long to wait on hardware before aborting
       *  @return Chimera::Status_t
       *
       *  |   Return Value   |                  Explanation                  |
       *  |:----------------:|:---------------------------------------------:|
       *  |               OK | Everything worked as expected                 |
       *  |             FAIL | The function failed                           |
       *  |  NOT_INITIALIZED | The serial hardware is not ready              |
       *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function |
       */
      virtual Chimera::Status_t write( const uint8_t *const buffer, const size_t length, const uint32_t timeout_mS = 500 ) = 0;

      /**
       *  Read an exact number of bytes from the wire
       *
       *  @note Depending on the hardware mode, this function will behave a bit differently.
       *  @warning Interrupt and DMA mode only work if you give the software a circular buffer for asynchronous transfers
       *
       *  #### Blocking Mode
       *    The function won't return until the number of bytes specified has been received
       *    or the timeout has occurred. The data will be placed directly into the given buffer
       *    and nothing will be copied to the user's circular buffer if it exists.
       *
       *  #### Interrupt & DMA
       *    The function immediately returns after queuing up the reception. The implementation software can
       *    use the given buffer as a temporary storage location or simply ignored. Once the tranfer is complete,
       *    received data is dumped into the circular buffer, which can be read using readAsync(). The user can
       *    then be notified of completion by either an event notifier or by checking the available() function.
       *
       *  @see enableBuffering
       *  @see attachEventNotifier
       *
       *  @param[in]  buffer        In blocking mode, will contain data read from RX
       *  @param[in]  length        How many bytes to read
       *  @param[in]  timeout_mS    How long to wait on hardware before aborting
       *  @return Chimera::Status_t
       *
       *  |   Return Value   |                  Explanation                  |
       *  |:----------------:|:---------------------------------------------:|
       *  |               OK | Everything worked as expected                 |
       *  |             FAIL | The function failed                           |
       *  |  NOT_INITIALIZED | The serial hardware is not ready              |
       *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function |
       */
      virtual Chimera::Status_t read( uint8_t *const buffer, const size_t length, const uint32_t timeout_mS = 500 ) = 0;

      /**
       *	Flushes out the given subperipheral queues
       *
       *	@param[in]	periph        The peripheral to be flushed
       *	@return Chimera::Status_t
       *
       *  | Return Value |          Explanation          |
       *  |:------------:|:-----------------------------:|
       *  |           OK | Everything worked as expected |
       *  |         FAIL | The function failed           |
       */
      virtual Chimera::Status_t flush( const SubPeripheral periph ) = 0;

      /**
       *  Read data queued from the RX buffer. This buffer can only be filled if the
       *  hardware was placed in Interrupt or DMA RX mode.
       *
       *  @param[in]  buffer        Array to store the data into
       *  @param[in]  len           The number of bytes to read from the RX buffer
       *  @return Chimera::Status_t
       *
       *  |   Return Value   |                   Explanation                  |
       *  |:----------------:|:----------------------------------------------:|
       *  |               OK | Everything worked as expected                  |
       *  |            EMPTY | There were not enough queued bytes to read out |
       *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function  |
       */
      virtual Chimera::Status_t readAsync( uint8_t *const buffer, const size_t len )
      {
        return Status::NOT_SUPPORTED;
      }

      /**
       *  Turns on buffering for asynchronous modes (Interrupt, DMA)
       *
       *  This helps prevent losing data when another device randomly sends over some bits without the caller expecting
       *  it, or when there is lots of traffic and code can't react fast enough.
       *
       *  @param[in]  periph        The peripheral (TX or RX) to buffer on
       *  @param[in]  buffer        The circular buffer to buffer data with
       *  @return Chimera::Status_t
       *
       *  |   Return Value   |                       Explanation                      |
       *  |:----------------:|:------------------------------------------------------:|
       *  |               OK | Everything worked as expected                          |
       *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function          |
       *  |    NOT_SUPPORTED | This function is not supported by the low level driver |
       */
      virtual Chimera::Status_t enableBuffering( const SubPeripheral periph, boost::circular_buffer<uint8_t> *const buffer )
      {
        return Status::NOT_SUPPORTED;
      }

      /**
       *  Turns off the buffering feature
       *
       *  @note This will automatically transition the given sub-peripheral back to blocking mode
       *
       *  @param[in]  periph        The peripheral to disable buffering on
       *  @return Chimera::Status_t
       *
       *  |   Return Value   |                       Explanation                      |
       *  |:----------------:|:------------------------------------------------------:|
       *  |               OK | Everything worked as expected                          |
       *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function          |
       *  |    NOT_SUPPORTED | This function is not supported by the low level driver |
       */
      virtual Chimera::Status_t disableBuffering( const SubPeripheral periph )
      {
        return Status::NOT_SUPPORTED;
      }

      /**
       *  Check if data is available to be read. Only works when configured in Interrupt or DMA mode.
       *
       *  @param[in]  bytes       Optionally report back how many bytes are ready
       *  @return bool
       *
       *  | Return Value |      Explanation     |
       *  |:------------:|:--------------------:|
       *  |         true | Data is available    |
       *  |        false | No data is available |
       */
      virtual bool available( size_t *const bytes = nullptr )
      {
        return false;
      }

      /**
       *  Attach a signal to get notified when an event occurs
       *
       *  @param[in]  event         The event to be notified on
       *  @param[in]  notifier      The notification variable
       *  @return Chimera::Status_t
       *
       *  |   Return Value   |                       Explanation                      |
       *  |:----------------:|:------------------------------------------------------:|
       *  |               OK | Everything worked as expected                          |
       *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function          |
       *  |    NOT_SUPPORTED | This function is not supported by the low level driver |
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
       *   @return Chimera::Status_t
       *
       *  |   Return Value   |                       Explanation                      |
       *  |:----------------:|:------------------------------------------------------:|
       *  |               OK | Everything worked as expected                          |
       *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function          |
       *  |    NOT_SUPPORTED | This function is not supported by the low level driver |
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
       *   @return Chimera::Status_t
       *
       *  |   Return Value   |                       Explanation                      |
       *  |:----------------:|:------------------------------------------------------:|
       *  |               OK | Everything worked as expected                          |
       *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function          |
       *  |    NOT_SUPPORTED | This function is not supported by the low level driver |
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
       *   @return Chimera::Status_t
       *
       *  |   Return Value   |                       Explanation                      |
       *  |:----------------:|:------------------------------------------------------:|
       *  |               OK | Everything worked as expected                          |
       *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function          |
       *  |    NOT_SUPPORTED | This function is not supported by the low level driver |
       */
      virtual Chimera::Status_t removeEventNotifier( const Event event, SemaphoreHandle_t *const semphr )
      {
        return Status::NOT_SUPPORTED;
      }
#endif
    };

    class SerialUnsupported : public Interface
    {
    public:
      SerialUnsupported( const size_t bufferSize )
      {
      }

      virtual Chimera::Status_t assignHW( const uint8_t channel, const IOPins &pins ) override
      {
        return Chimera::CommonStatusCodes::FAIL;
      }

      virtual Chimera::Status_t begin( const Modes txMode, const Modes rxMode ) override
      {
        return Chimera::CommonStatusCodes::FAIL;
      }

      virtual Chimera::Status_t end() override
      {
        return Chimera::CommonStatusCodes::FAIL;
      }

      virtual Chimera::Status_t configure( const uint32_t baud, const CharWid width, const Parity parity, const StopBits stop,
                                           const FlowControl flow ) override
      {
        return Chimera::CommonStatusCodes::FAIL;
      }

      virtual Chimera::Status_t setBaud( const uint32_t baud ) override
      {
        return Chimera::CommonStatusCodes::FAIL;
      }

      virtual Chimera::Status_t setMode( const SubPeripheral periph, const Modes mode ) override
      {
        return Chimera::CommonStatusCodes::FAIL;
      }

      virtual Chimera::Status_t write( const uint8_t *const buffer, const size_t length,
                                       const uint32_t timeout_mS = 500 ) override
      {
        return Chimera::CommonStatusCodes::FAIL;
      }

      virtual Chimera::Status_t read( uint8_t *const buffer, const size_t length, const uint32_t timeout_mS = 500 ) override
      {
        return Chimera::CommonStatusCodes::FAIL;
      }

      virtual Chimera::Status_t flush( const SubPeripheral periph ) override
      {
        return Chimera::CommonStatusCodes::FAIL;
      }

      virtual Chimera::Status_t readAsync( uint8_t *const buffer, const size_t len ) override
      {
        return Status::NOT_SUPPORTED;
      }

      virtual Chimera::Status_t enableBuffering( const SubPeripheral periph,
                                                 boost::circular_buffer<uint8_t> *const buffer ) override
      {
        return Status::NOT_SUPPORTED;
      }

      virtual Chimera::Status_t disableBuffering( const SubPeripheral periph ) override
      {
        return Status::NOT_SUPPORTED;
      }

      virtual bool available( size_t *const bytes = nullptr ) override
      {
        return false;
      }

      virtual Chimera::Status_t attachEventNotifier( const Event event, volatile bool *const notifier ) override
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }

      virtual Chimera::Status_t removeEventNotifier( const Event event, volatile bool *const notifier ) override
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }

#if defined( USING_FREERTOS )
      virtual Chimera::Status_t attachEventNotifier( const Event event, SemaphoreHandle_t *const semphr ) override
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }

      virtual Chimera::Status_t removeEventNotifier( const Event event, SemaphoreHandle_t *const semphr ) override
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }
#endif /* USING_FREERTOS */
    };

  }  // namespace Serial
}  // namespace Chimera

#endif /* !CHIMERA_SERIAL_INTERFACE_HPP */