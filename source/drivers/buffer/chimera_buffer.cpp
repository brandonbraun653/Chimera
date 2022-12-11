/********************************************************************************
 *  File Name:
 *    chimera_buffer.cpp
 *
 *  Description:
 *    Models Chimera buffer related interfaces
 *
 *  2019-2021 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* STL Includes */
#include <cstring>

/* Boost Includes */
#include <etl/circular_buffer.h>

/* Chimera Includes */
#include <Chimera/buffer>


namespace Chimera::Buffer
{
  PeripheralBuffer::PeripheralBuffer() : pLinearBuffer( nullptr ), linearLength( 0 ), pCircularBuffer( nullptr )
  {
  }


  PeripheralBuffer::~PeripheralBuffer()
  {
  }


  bool PeripheralBuffer::initialized()
  {
    return ( pLinearBuffer && pCircularBuffer );
  }


  Chimera::Status_t PeripheralBuffer::assign( Chimera::Serial::BipBuffer &circularBuffer, uint8_t *const linearBuffer,
                                              const size_t linearSize )
  {
    using namespace Chimera::Thread;

    if ( !linearBuffer || !linearSize )
    {
      return Chimera::Status::INVAL_FUNC_PARAM;
    }

    if ( TimedLockGuard( *this ).try_lock_for( 10 ) )
    {
      /*-------------------------------------------------
      The external memory may be dynamically allocated, but we
      don't manage it so set the 'dynamic' flag to false.
      -------------------------------------------------*/
      pLinearBuffer   = linearBuffer;
      linearLength    = linearSize;
      pCircularBuffer = &circularBuffer;
    }

    return Chimera::Status::OK;
  }


  Chimera::Status_t PeripheralBuffer::push( const uint8_t *const buffer, const size_t len, size_t &actual )
  {
    using namespace Chimera::Hardware;
    using namespace Chimera::Thread;

    auto error          = Chimera::Status::LOCKED;
    size_t bytesWritten = 0;
    actual              = 0;

    /*-------------------------------------------------
    Input protection
    -------------------------------------------------*/
    if ( !buffer || !len )
    {
      return Chimera::Status::INVAL_FUNC_PARAM;
    }
    else if ( !pCircularBuffer )
    {
      return Chimera::Status::NOT_INITIALIZED;
    }

    /*-------------------------------------------------
    Make sure we can safely access the data
    -------------------------------------------------*/
    if ( TimedLockGuard( *this ).try_lock_for( 10 ) )
    {
      error = Chimera::Status::OK;

      while ( pCircularBuffer->available() && ( bytesWritten < len ) )
      {
        pCircularBuffer->push( buffer[ bytesWritten ] );
        bytesWritten++;
      }

      actual = bytesWritten;
      if ( bytesWritten != len )
      {
        error = Chimera::Status::FULL;
      }
    }

    return error;
  }


  Chimera::Status_t PeripheralBuffer::pop( uint8_t *const buffer, const size_t len, size_t &actual )
  {
    using namespace Chimera::Hardware;
    using namespace Chimera::Thread;

    auto error       = Chimera::Status::LOCKED;
    size_t bytesRead = 0;
    actual           = 0;

    /*-------------------------------------------------
    Input protection
    -------------------------------------------------*/
    if ( !buffer || !len )
    {
      return Chimera::Status::INVAL_FUNC_PARAM;
    }
    else if ( !pCircularBuffer )
    {
      return Chimera::Status::NOT_INITIALIZED;
    }

    /*-------------------------------------------------
    Make sure we can safely access the data
    -------------------------------------------------*/
    if ( TimedLockGuard( *this ).try_lock_for( 10 ) )
    {
      error = Chimera::Status::OK;

      while ( !pCircularBuffer->empty() && ( bytesRead < len ) )
      {
        buffer[ bytesRead ] = pCircularBuffer->front();
        pCircularBuffer->pop();
        bytesRead++;
      }

      actual = bytesRead;
      if ( bytesRead != len )
      {
        error = Chimera::Status::EMPTY;
      }
    }

    return error;
  }


  Chimera::Status_t PeripheralBuffer::flush()
  {
    using namespace Chimera::Thread;
    auto error = Chimera::Status::LOCKED;

    if ( !pLinearBuffer || !pCircularBuffer )
    {
      return Chimera::Status::NOT_INITIALIZED;
    }

    if ( TimedLockGuard( *this ).try_lock_for( 10 ) )
    {
      error = Chimera::Status::OK;
      memset( pLinearBuffer, 0, linearLength );
      pCircularBuffer->clear();
    }

    return error;
  }


  Chimera::Status_t PeripheralBuffer::transferInto( const size_t bytes, size_t &actual )
  {
    using namespace Chimera::Thread;

    auto result        = Chimera::Status::LOCKED;
    size_t bytesToCopy = 0;
    size_t bytesCopied = 0;
    actual             = 0;

    if ( !initialized() )
    {
      return Chimera::Status::NOT_INITIALIZED;
    }

    if ( TimedLockGuard( *this ).try_lock_for( 10 ) )
    {
      result = Chimera::Status::OK;

      /*-------------------------------------------------
      Assume we are going to copy everything
      -------------------------------------------------*/
      bytesToCopy = pCircularBuffer->size();

      /*-------------------------------------------------
      Force linear array overrun protection
      -------------------------------------------------*/
      if ( bytesToCopy > linearLength )
      {
        bytesToCopy = linearLength;
        result      = Chimera::Status::FULL;
      }

      /*-------------------------------------------------
      Truncate the number of bytes we try and copy from
      the circular buffer
      -------------------------------------------------*/
      if ( bytesToCopy > bytes )
      {
        bytesToCopy = bytes;
      }

      /*-------------------------------------------------
      Perform the copy operation
      -------------------------------------------------*/
      while ( bytesCopied < bytesToCopy )
      {
        pLinearBuffer[ bytesCopied ] = pCircularBuffer->front();
        pCircularBuffer->pop();
        bytesCopied++;
      }

      actual = bytesCopied;
    }

    return result;
  }


  Chimera::Status_t PeripheralBuffer::transferOutOf( const size_t bytes, size_t &actual )
  {
    using namespace Chimera::Thread;

    auto result             = Chimera::Status::LOCKED;
    size_t bytesToCopy      = 0;
    size_t bytesbytesCopied = 0;
    size_t remainingSpace   = 0;
    actual                  = 0;

    if ( !initialized() )
    {
      return Chimera::Status::NOT_INITIALIZED;
    }

    if ( TimedLockGuard( *this ).try_lock_for( 10 ) )
    {
      result = Chimera::Status::OK;

      bytesToCopy    = bytes;
      remainingSpace = pCircularBuffer->available();

      /*-------------------------------------------------
      Force linear array overrun protection
      -------------------------------------------------*/
      if ( bytesToCopy > linearLength )
      {
        bytesToCopy = linearLength;
      }

      /*-------------------------------------------------
      Make sure we don't try and push too many bytes
      -------------------------------------------------*/
      if ( bytesToCopy > remainingSpace )
      {
        bytesToCopy = remainingSpace;
        result      = Chimera::Status::FULL;
      }

      /*-------------------------------------------------
      Perform the transfer
      -------------------------------------------------*/
      while ( bytesToCopy > 0 )
      {
        pCircularBuffer->push( pLinearBuffer[ bytesbytesCopied ] );
        bytesbytesCopied++;
        bytesToCopy--;
      }

      actual = bytesbytesCopied;
    }

    return result;
  }


  Chimera::Serial::BipBuffer *PeripheralBuffer::circularBuffer()
  {
    return pCircularBuffer;
  }


  uint8_t *const PeripheralBuffer::linearBuffer()
  {
    return pLinearBuffer;
  }


  size_t PeripheralBuffer::linearSize()
  {
    return linearLength;
  }

}  // namespace Chimera::Buffer
