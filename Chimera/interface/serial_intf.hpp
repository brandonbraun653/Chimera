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

/* Boost Includes */
#include <boost/circular_buffer_fwd.hpp>

/* Chimera Includes */
#include <Chimera/types/callback_types.hpp>
#include <Chimera/types/common_types.hpp>
#include <Chimera/types/serial_types.hpp>


namespace Chimera::Serial
{
  class HWInterface
  {
  public:
    virtual ~HWInterface() = default;

    /**
     *  Attaches and configures the physical hardware channel and GPIO pin setup
     *
     *  @param[in]  channel       The physical UART hardware channel to use
     *  @param[in]  pins          TX and RX pin configuration
     *  @return Chimera::Status_t
     *
     *  |   Return Value   |            Explanation             |
     *  |:----------------:|:----------------------------------:|
     *  |               OK | Everything worked as expected      |
     *  |             FAIL | The function failed                |
     *  | INVAL_FUNC_PARAM | An invalid parameter was passed in |
     */
    virtual Chimera::Status_t assignHW( const uint8_t channel, const IOPins &pins ) = 0;

    /**
     *  Starts up the Serial interface with a baud rate and transfer mode
     *
     *  @param[in]  txMode        What mode to run the TX hardware in
     *  @param[in]  rxMode        What mode to run the RX hardware in
     *  @return Chimera::Status_t
     *
     *  |   Return Value   |            Explanation             |
     *  |:----------------:|:----------------------------------:|
     *  |               OK | Everything worked as expected      |
     *  |             FAIL | The function failed                |
     *  |  NOT_INITIALIZED | The serial hardware has not been configured properly |
     *  | INVAL_FUNC_PARAM | An invalid parameter was passed in |
     */
    virtual Chimera::Status_t begin( const Chimera::Hardware::PeripheralMode txMode,
                                     const Chimera::Hardware::PeripheralMode rxMode ) = 0;

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
     *  @param[in]  config        The configuration parameters 
     *  @return Chimera::Status_t
     *
     *  |   Return Value   |                     Explanation                    |
     *  |:----------------:|:--------------------------------------------------:|
     *  |               OK | Everything worked as expected                      |
     *  |             FAIL | The function failed configuration of the hardware  |
     *  |  NOT_INITIALIZED | The serial hardware is not ready for configuration |
     *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function      |
     */
    virtual Chimera::Status_t configure( const Config &config ) = 0;

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
    virtual Chimera::Status_t setMode( const Chimera::Hardware::SubPeripheral periph,
                                       const Chimera::Hardware::PeripheralMode mode ) = 0;

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
     *	Flushes out the given sub peripheral queues
     *
     *	@param[in]	periph        The peripheral to be flushed
     *	@return Chimera::Status_t
     *
     *  | Return Value |          Explanation          |
     *  |:------------:|:-----------------------------:|
     *  |           OK | Everything worked as expected |
     *  |         FAIL | The function failed           |
     */
    virtual Chimera::Status_t flush( const Chimera::Hardware::SubPeripheral periph ) = 0;

    /**
     *  A lot of the functionality of this class depends upon reacting to some kind of 
     *  interrupt or asynchronous event. Given that some non-trivial processing could
     *  occur in either a callback or by suddenly having to handle a large amount of
     *  data, this function allows that processing to occur in a non-ISR context.
     * 
     *  @note It is recommended to signal a high priority thread to wake up and call
     *        this function when the ISR exits. As such, there may be a slight delay
     *        between the signal and the time this function actually executes.
     * 
     *  @warning Under no circumstances should this function be called inside an ISR.
     *
     *	@return void
     */
    virtual void postISRProcessing() = 0;

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
    virtual Chimera::Status_t readAsync( uint8_t *const buffer, const size_t len ) = 0;

    /**
     *  Turns on buffering for asynchronous modes (Interrupt, DMA)
     *
     *  This helps prevent losing data when another device randomly sends over some bits without the caller expecting
     *  it, or when there is lots of traffic and code can't react fast enough.
     *
     *  @param[in]  periph        The peripheral (TX or RX) to buffer on
     *  @param[in]  userBuffer    The circular buffer that will contain user data
     *  @param[in]  hwBuffer      Buffer that the hardware uses to transmit and receive data
     *  @param[in]  hwBufferSize  The length of the hardware buffer
     *  @return Chimera::Status_t
     *
     *  |   Return Value   |                       Explanation                      |
     *  |:----------------:|:------------------------------------------------------:|
     *  |               OK | Everything worked as expected                          |
     *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function          |
     *  |    NOT_SUPPORTED | This function is not supported by the low level driver |
     */
    virtual Chimera::Status_t enableBuffering( const Chimera::Hardware::SubPeripheral periph,
                                               boost::circular_buffer<uint8_t> *const userBuffer, uint8_t *const hwBuffer,
                                               const uint32_t hwBufferSize ) = 0;

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
    virtual Chimera::Status_t disableBuffering( const Chimera::Hardware::SubPeripheral periph ) = 0;

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
    virtual bool available( size_t *const bytes = nullptr ) = 0;
  };

}  // namespace Chimera::Serial

#endif /* !CHIMERA_SERIAL_INTERFACE_HPP */