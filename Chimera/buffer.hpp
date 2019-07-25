/********************************************************************************
 *   File Name:
 *    buffer.hpp
 *
 *   Description:
 *    Models Chimera buffer related interfaces
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_BUFFER_HPP
#define CHIMERA_BUFFER_HPP

/* C++ Includes */
#include <cstring>
#include <memory>

/* Boost Includes */
#include <boost/circular_buffer_fwd.hpp>

/* Chimera Includes */
#include <Chimera/interface/buffer_intf.hpp>
#include <Chimera/threading.hpp>

namespace Chimera::Buffer
{
  /**
   *  A variant on the double buffering technique, optimized for hardware peripherals that
   *  operate asynchronously. It contains two buffers, a user level circular buffer that
   *  data can be dumped into by the user or hardware, and a classic linear buffer that only
   *  the hardware can use.
   *
   *  The goal here is to guarantee non-modifiable data for transactions in Interrupt or
   *  DMA mode but still allow the user to queue up more data if needed.
   */
  class DoubleBuffer : public Chimera::Threading::Lockable
  {
  public:
    DoubleBuffer();
    ~DoubleBuffer();

    /**
     *  Dynamically allocates the memory for the 
     *
     *  @param[in]  circularSize    Number of bytes for the circular buffer        
     *  @param[in]  linearSize      Number of bytes for the linear buffer
     *  @return Chimera::Status_t
     *
     *  |   Return Value   |                       Explanation                      |
     *  |:----------------:|:------------------------------------------------------:|
     *  |               OK | Everything worked as expected                          |
     *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function          |
     *  |    NOT_SUPPORTED | This function is not supported by the low level driver |
     */
    Chimera::Status_t assign( const size_t circularSize, const size_t linearSize );

    Chimera::Status_t assignStatic( boost::circular_buffer<uint8_t> *const circularBuffer, 
                                    uint8_t *const linearBuffer,
                                    const size_t linearSize );

    /**
     *  Push data from an array into the circular buffer
     *
     *  @param[in]  buffer        Array to push data from
     *  @param[in]  len           The number of bytes to push
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
    Chimera::Status_t push( const uint8_t *const buffer, const size_t len );

    /**
     *  Pop data from the circular buffer into an array
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
    Chimera::Status_t pop( uint8_t *const buffer, const size_t len );

    /**
     *  Flushes both of the buffers
     *
     *  @return Chimera::Status_t
     *
     *  |   Return Value   |                       Explanation                      |
     *  |:----------------:|:------------------------------------------------------:|
     *  |               OK | Everything worked as expected                          |
     *  |    NOT_SUPPORTED | This function is not supported by the low level driver |
     */
    Chimera::Status_t flush();

    /**
     *  Gets a pointer to the circular buffer
     *
     *  @return boost::circular_buffer<uint8_t> *const
     *
     *  |   Return Value   |                       Explanation                      |
     *  |:----------------:|:------------------------------------------------------:|
     *  |               OK | Everything worked as expected                          |
     *  |    NOT_SUPPORTED | This function is not supported by the low level driver |
     */
    boost::circular_buffer<uint8_t> *const circularBuffer();

    bool initialized();

    /**
     *  Transfers bytes from the circular buffer into the hardware buffer
     *
     *  @param[in]  bytes   The number of bytes to try and copy
     *  @return size_t      The number of bytes actually copied
     */
    size_t transferInto( const size_t bytes );

  private:
    bool dynamicData;
    uint8_t *pLinearBuffer;
    size_t linearLength;
    boost::circular_buffer<uint8_t> *pCircularBuffer;

    void freeDynamicData();
  };

}  // namespace Chimera::Buffer

#endif /* !CHIMERA_BUFFER_HPP */