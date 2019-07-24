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

/* Boost Includes */
#include <boost/circular_buffer.hpp>

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
  template<class Hardware>
  class DoubleBuffer : public Chimera::Threading::Lockable
  {
  public:
    friend Hardware;

    DoubleBuffer()
    {
      external     = nullptr;
      internal     = nullptr;
      internalSize = 0;
    }
    ~DoubleBuffer() = default;

    /**
     *  Push data into the circular buffer
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
    Chimera::Status_t push( const uint8_t *const buffer, const size_t len )
    {
      using namespace Chimera::Hardware;

      Chimera::Status_t error = Chimera::CommonStatusCodes::OK;

      if ( !buffer )
      {
        error = Chimera::CommonStatusCodes::INVAL_FUNC_PARAM;
      }
      else
      {
        if ( lock( 10 ) == Chimera::CommonStatusCodes::OK )
        {
          size_t bytesWritten = 0;

          while ( !external->full() && ( bytesWritten < len ) )
          {
            external->push_back( buffer[ bytesWritten ] );
            bytesWritten++;
          }

          if ( bytesWritten != len )
          {
            error = Chimera::CommonStatusCodes::FULL;
          }

          unlock();
        }
        else
        {
          error = Chimera::CommonStatusCodes::LOCKED;
        }
      }

      return error;
    }

    /**
     *  Read data from the circular buffer
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
    Chimera::Status_t pop( uint8_t *const buffer, const size_t len )
    {
      using namespace Chimera::Hardware;

      Chimera::Status_t error = Chimera::CommonStatusCodes::OK;

      if ( !buffer )
      {
        error = Chimera::CommonStatusCodes::INVAL_FUNC_PARAM;
      }
      else
      {
        if ( lock( Chimera::Threading::TIMEOUT_DONT_WAIT ) == Chimera::CommonStatusCodes::OK )
        {
          size_t bytesRead = 0;

          if ( external )
          {
            while ( !external->empty() && ( bytesRead < len ) )
            {
              buffer[ bytesRead ] = external->front();
              external->pop_front();
              bytesRead++;
            }
          }

          if ( bytesRead != len )
          {
            error = Chimera::CommonStatusCodes::EMPTY;
          }

          unlock();
        }
        else
        {
          error = Chimera::CommonStatusCodes::LOCKED;
        }
      }

      return error;
    }

    /**
     *  Assigns the buffers to be used
     *
     *  @param[in]  periph        The sub peripheral to assign the external buffer to
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
    Chimera::Status_t assign( boost::circular_buffer<uint8_t> *const userBuffer, uint8_t *const hwBuffer,
                              const uint32_t hwBuffersize )
    {
      if ( lock( Chimera::Threading::TIMEOUT_DONT_WAIT ) == Chimera::CommonStatusCodes::OK )
      {
        internal     = hwBuffer;
        internalSize = hwBuffersize;
        external     = userBuffer;

        unlock();
      }

      return Chimera::CommonStatusCodes::OK;
    }

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
    Chimera::Status_t flush()
    {
      Chimera::Status_t error = Chimera::CommonStatusCodes::OK;

      if ( lock( Chimera::Threading::TIMEOUT_DONT_WAIT ) == Chimera::CommonStatusCodes::OK )
      {
        if ( internal )
        {
          memset( internal, 0, internalSize );
        }
        else
        {
          error = Chimera::CommonStatusCodes::FAIL;
        }

        if ( external )
        {
          external->clear();
        }
        else
        {
          error = Chimera::CommonStatusCodes::FAIL;
        }

        unlock();
      }
      else
      {
        error = Chimera::CommonStatusCodes::LOCKED;
      }

      return error;
    }

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
    boost::circular_buffer<uint8_t> *const get()
    {
      return external;
    }

    bool initialized()
    {
      return ( internal && external );
    }


    /**
     *  Transfers bytes from the circular buffer into the hardware buffer
     *
     *  @param[in]  bytes   The number of bytes to try and copy
     *  @return size_t      The number of bytes actually copied
     */
    size_t copyToHWBuffer( const size_t bytes )
    {
      size_t bytesToCopy = 0;

      if ( lock( Chimera::Threading::TIMEOUT_DONT_WAIT ) == Chimera::CommonStatusCodes::OK )
      {
        bytesToCopy = external->size();

        if ( bytesToCopy > internalSize )
        {
          bytesToCopy = internalSize;
        }

        if ( bytesToCopy > bytes )
        {
          bytesToCopy = bytes;
        }

        size_t copied = 0;

        while ( copied < bytesToCopy )
        {
          internal[ copied ] = external->front();
          external->pop_front();
          copied++;
        }

        unlock();
      }

      return bytesToCopy;
    }

  protected:
    uint8_t *getHWBuffer()
    {
      return internal;
    }

    boost::circular_buffer<uint8_t> *external;
    uint8_t *internal;
    uint32_t internalSize;
  };

}  // namespace Chimera::Buffer

#endif /* !CHIMERA_BUFFER_HPP */