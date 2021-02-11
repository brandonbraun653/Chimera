/********************************************************************************
 *  File Name:
 *    buffer.hpp
 *
 *  Description:
 *    Models Chimera buffer related interfaces
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_BUFFER_HPP
#define CHIMERA_BUFFER_HPP

/* STL Includes */
#include <cstring>
#include <memory>

/* Boost Includes */
#include <boost/circular_buffer_fwd.hpp>

/* Chimera Includes */
#include <Chimera/source/drivers/buffer/buffer_intf.hpp>
#include <Chimera/thread>

namespace Chimera::Buffer
{
  /**
   *  A variant on the double buffering technique, optimized for hardware peripherals that
   *  operate asynchronously. It contains two buffers, a circular buffer that data can be
   *  dumped into by the user or hardware, and a linear buffer that only the hardware should
   *  use.
   *
   *  The purpose is to guarantee non-modifiable data for hardware transactions that utilize
   *  interrupts or DMA, but still allow the user to queue up more data as needed. This helps
   *  with smooth asynchronous operation.
   */
  class PeripheralBuffer : public Chimera::Thread::Lockable<PeripheralBuffer>
  {
  public:
    PeripheralBuffer();
    ~PeripheralBuffer();

    /**
     *  Checks if the buffer assignment has completed successfully, indicating
     *  that the object is ready to be used.
     *
     *  @return bool
     */
    bool initialized();

    /**
     *  Dynamically allocates memory for the double buffer
     *
     *  @param[in]  circularSize    Number of bytes for the circular buffer
     *  @param[in]  linearSize      Number of bytes for the linear buffer
     *  @return Chimera::Status_t
     *
     *  |   Return Value   |              Explanation             |
     *  |:----------------:|:------------------------------------:|
     *  |               OK | The memory was assigned successfully |
     *  | INVAL_FUNC_PARAM | An invalid parameter was passed in   |
     *  |           LOCKED | The buffers are currently locked     |
     */
    Chimera::Status_t assign( const size_t circularSize, const size_t linearSize );

    /**
     *  Assigns the double buffer using external memory
     *
     *  @param[in]  circularBuffer    The circular buffer to be managed
     *  @param[in]  linearBuffer      The linear buffer to be managed
     *  @param[in]  linearSize        The size of the linear buffer in bytes
     *  @return Chimera::Status_t
     *
     *  |   Return Value   |              Explanation             |
     *  |:----------------:|:------------------------------------:|
     *  |               OK | The memory was assigned successfully |
     *  | INVAL_FUNC_PARAM | An invalid parameter was passed in   |
     *  |           LOCKED | The buffers are currently locked     |
     */
    Chimera::Status_t assign( boost::circular_buffer<uint8_t> *const circularBuffer, uint8_t *const linearBuffer,
                              const size_t linearSize );

    /**
     *  Push data from some memory into the circular buffer
     *
     *  @param[in]  buffer        Memory to push data from
     *  @param[in]  len           The number of bytes to push
     *  @return Chimera::Status_t
     *
     *  |   Return Value   |                   Explanation                  |
     *  |:----------------:|:----------------------------------------------:|
     *  |               OK | Everything worked as expected                  |
     *  |            EMPTY | There was not enough space left in the buffer  |
     *  |  NOT_INITIALIZED | The buffer has not been initialized yet        |
     *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function  |
     *  |           LOCKED | The buffers are currently locked               |
     */
    Chimera::Status_t push( const uint8_t *const buffer, const size_t len, size_t &actual );

    /**
     *  Pop data from the circular buffer into the provided memory
     *
     *  @param[in]  buffer        Memory to store the data into. Must be able to handle 'len' bytes.
     *  @param[in]  len           The number of bytes to read from the circular buffer
     *  @param[out] actual        Actual number of bytes read
     *  @return Chimera::Status_t
     *
     *  |   Return Value   |                   Explanation                  |
     *  |:----------------:|:----------------------------------------------:|
     *  |               OK | Everything worked as expected                  |
     *  |            EMPTY | There were not enough queued bytes to read out |
     *  |  NOT_INITIALIZED | The buffer has not been initialized yet        |
     *  | INVAL_FUNC_PARAM | A bad parameter was passed in to the function  |
     *  |           LOCKED | The buffers are currently locked               |
     */
    Chimera::Status_t pop( uint8_t *const buffer, const size_t len, size_t &actual );

    /**
     *  Flushes both the linear and circular buffers.
     *
     *  @return Chimera::Status_t
     *
     *  |   Return Value  |                     Explanation                    |
     *  |:---------------:|:--------------------------------------------------:|
     *  |              OK | The memory was cleared successfully                |
     *  |          LOCKED | The buffers are currently locked by another thread |
     *  | NOT_INITIALIZED | The buffers haven't been assigned yet              |
     */
    Chimera::Status_t flush();

    /**
     *  Internally moves bytes from the circular buffer into the linear buffer
     *
     *  @param[in]  bytes   The number of bytes to try and copy
     *  @param[out] actual  The number of bytes actually copied
     *  @return Chimera::Status_t
     *
     *  |   Return Value  |                     Explanation                    |
     *  |:---------------:|:--------------------------------------------------:|
     *  |              OK | The data was copied successfully                   |
     *  |           EMPTY | The source memory ran out of bytes                 |
     *  |            FULL | The destination memory ran out of space            |
     *  |          LOCKED | The buffers are currently locked by another thread |
     *  | NOT_INITIALIZED | The buffers haven't been assigned yet              |
     */
    Chimera::Status_t transferInto( const size_t bytes, size_t &actual );

    /**
     *  Internally moves bytes from the linear buffer into the circular buffer
     *
     *  @param[in]  bytes   The number of bytes to try and copy
     *  @param[out] actual  The number of bytes actually copied
     *  @return Chimera::Status_t
     *
     *  |   Return Value  |                     Explanation                    |
     *  |:---------------:|:--------------------------------------------------:|
     *  |              OK | The data was copied successfully                   |
     *  |           EMPTY | The source memory ran out of bytes                 |
     *  |            FULL | The destination memory ran out of space            |
     *  |          LOCKED | The buffers are currently locked by another thread |
     *  | NOT_INITIALIZED | The buffers haven't been assigned yet              |
     */
    Chimera::Status_t transferOutOf( const size_t bytes, size_t &actual );

    /**
     *  Gets the internal pointer to the circular buffer
     *
     *  @return boost::circular_buffer<uint8_t> *const
     */
    boost::circular_buffer<uint8_t> *const circularBuffer();

    /**
     *  Gets the internal pointer to the linear buffer
     *
     *  @return uint8_t *const
     */
    uint8_t *const linearBuffer();

    /**
     *  Gets the length of the linear buffer
     *
     *  @return size_t
     */
    size_t linearSize();

  private:
    friend Chimera::Thread::Lockable<PeripheralBuffer>;


    bool dynamicData;
    uint8_t *pLinearBuffer;
    size_t linearLength;
    boost::circular_buffer<uint8_t> *pCircularBuffer;

    void freeDynamicData();
  };

}  // namespace Chimera::Buffer

#endif /* !CHIMERA_BUFFER_HPP */
