/********************************************************************************
 *  File Name:
 *    bufferable_intf.hpp
 *
 *  Description:
 *    Models the Chimera Bufferable interface
 *
 *  2019-2021 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_BUFFERABLE_INTERFACE_HPP
#define CHIMERA_BUFFERABLE_INTERFACE_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/serial>

namespace Chimera::Buffer
{
  /**
   * A higher level class that allows public access to multiple internal double buffer objects.
   * Meant to be used in conjunction with Chimera::Buffer::DoubleBuffer.
   */
  class DoubleBufferableInterface
  {
  public:
    virtual ~DoubleBufferableInterface() = default;

    /**
     *  Push data into the TX buffer
     *
     *  @param[in]  buffer        Array to store the data into
     *  @param[in]  len           The number of bytes to read from the RX buffer
     *  @return Chimera::Status_t
     *
     *  |   Return Value   |                   Explanation                  |
     *  |:----------------:|:----------------------------------------------:|
     *  |               OK | Everything worked as expected                  |
     *  |            EMPTY | There was not enough space left in the buffer  |
     *  |  NOT_INITIALIZED | The buffer has not been initialized yet        |
     *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function  |
     *  |    NOT_SUPPORTED | This functionality isn't supported             |
     */
    virtual Chimera::Status_t bufferPush( const Chimera::Hardware::SubPeripheral periph, uint8_t *const buffer,
                                          const size_t len ) = 0;

    /**
     *  Read queued data from the RX buffer. This buffer can only be filled if the
     *  hardware was placed in Interrupt or DMA mode.
     *
     *  @param[in]  buffer        Array to store the data into
     *  @param[in]  len           The number of bytes to read from the RX buffer
     *  @return Chimera::Status_t
     *
     *  |   Return Value   |                   Explanation                  |
     *  |:----------------:|:----------------------------------------------:|
     *  |               OK | Everything worked as expected                  |
     *  |            EMPTY | There were not enough queued bytes to read out |
     *  |  NOT_INITIALIZED | The buffer has not been initialized yet        |
     *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function  |
     *  |    NOT_SUPPORTED | This functionality isn't supported             |
     */
    virtual Chimera::Status_t bufferPop( const Chimera::Hardware::SubPeripheral periph, uint8_t *const buffer,
                                         const size_t len ) = 0;

    /**
     *  Turns on buffering for asynchronous modes (Interrupt, DMA). It will not transition
     *  the hardware to an asynchronous mode, only prepare some of the needed buffer resources.
     *
     *  This helps prevent losing data when another device randomly sends over some bits without the caller expecting
     *  it, or when there is lots of traffic and code can't react fast enough.
     *
     *  @param[in]  periph        The peripheral (TX or RX) to enable buffering on
     *  @return Chimera::Status_t
     *
     *  |   Return Value   |                       Explanation                      |
     *  |:----------------:|:------------------------------------------------------:|
     *  |               OK | Everything worked as expected                          |
     *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function          |
     *  |    NOT_SUPPORTED | This function is not supported by the low level driver |
     */
    virtual Chimera::Status_t bufferEnable( const Chimera::Hardware::SubPeripheral periph ) = 0;

    /**
     *  Turns off the buffering feature
     *
     *  @note This will automatically transition the given sub-peripheral back to blocking mode if supported
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
    virtual Chimera::Status_t bufferDisable( const Chimera::Hardware::SubPeripheral periph ) = 0;

    /**
     *  Assigns the internal buffer used for actually transfering data on the wire. This
     *  is separate from the external buffer which the user can interact with directly.
     *  Double buffering is used to allow more data to be queued up while the previous
     *  transfer is occuring.
     *
     *  @param[in]  periph        The sub peripheral to assign the interal buffer to
     *  @param[in]  buffer        The buffer that will be used for internal purposes
     *  @param[in]  size          The buffer's size
     *  @return Chimera::Status_t
     *
     *  |   Return Value   |                       Explanation                      |
     *  |:----------------:|:------------------------------------------------------:|
     *  |               OK | Everything worked as expected                          |
     *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function          |
     *  |    NOT_SUPPORTED | This function is not supported by the low level driver |
     */
    virtual Chimera::Status_t bufferAssignInternal( const Chimera::Hardware::SubPeripheral periph, uint8_t *const buffer,
                                                    const uint16_t size ) = 0;

    /**
     *  Assigns the external buffer the user will interact with to push/pop data. This
     *  is separate from the internal buffer which is not accessable to the user.
     *  Double buffering is used to allow more data to be queued up while the previous
     *  transfer is occuring.
     *
     *  @param[in]  periph        The sub peripheral to assign the external buffer to
     *  @param[in]  buffer        The buffer that the user can push/pop data onto
     *  @return Chimera::Status_t
     *
     *  |   Return Value   |                       Explanation                      |
     *  |:----------------:|:------------------------------------------------------:|
     *  |               OK | Everything worked as expected                          |
     *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function          |
     *  |    NOT_SUPPORTED | This function is not supported by the low level driver |
     */
    virtual Chimera::Status_t bufferAssignExternal( const Chimera::Hardware::SubPeripheral periph,
                                                    Chimera::Serial::CircularBuffer & buffer ) = 0;

    virtual Chimera::Status_t bufferFlush( const Chimera::Hardware::SubPeripheral periph ) = 0;
  };
}  // namespace Chimera::Buffer

#endif /* !CHIMERA_BUFFERABLE_INTERFACE_HPP */
