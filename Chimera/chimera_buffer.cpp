/********************************************************************************
 *   File Name:
 *    chimera_buffer.cpp
 *
 *   Description:
 *    Models Chimera buffer related interfaces
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */
#include <cstring>

/* Boost Includes */
#include <boost/circular_buffer.hpp>

/* Chimera Includes */
#include <Chimera/buffer.hpp>

namespace Chimera::Buffer
{
  DoubleBuffer::DoubleBuffer() : dynamicData( false ), pLinearBuffer( nullptr ), linearLength( 0 ), pCircularBuffer( nullptr )
  {
  }

  DoubleBuffer::~DoubleBuffer()
  {
    freeDynamicData();
  }

  Chimera::Status_t DoubleBuffer::assign( const size_t circularSize, const size_t linearSize )
  {
    if( !circularSize || !linearSize )
    {
      return Chimera::CommonStatusCodes::INVAL_FUNC_PARAM;
    }

    dynamicData = true;

    /*-------------------------------------------------
    Allocate the memory for the arrays
    -------------------------------------------------*/
    pLinearBuffer = new uint8_t( linearSize );
    linearLength = linearSize;

    pCircularBuffer = new boost::circular_buffer<uint8_t>();
    pCircularBuffer->resize( circularSize, 0);

    /*-------------------------------------------------
    
    -------------------------------------------------*/
    if( !pLinearBuffer || !pCircularBuffer || ( pCircularBuffer->size() != circularSize ))
    {
      freeDynamicData();
      return Chimera::CommonStatusCodes::MEMORY;
    }

    return Chimera::CommonStatusCodes::OK;
  }

  Chimera::Status_t DoubleBuffer::assignStatic( boost::circular_buffer<uint8_t> *const circularBuffer, uint8_t *const linearBuffer,
                                          const size_t linearSize )
  {
    using namespace Chimera::Threading;

    if ( LockGuard(*this).lock() )
    {
      if( dynamicData )
      {
        freeDynamicData();
      }

      pLinearBuffer   = linearBuffer;
      linearLength    = linearSize;
      pCircularBuffer = circularBuffer;

      
      dynamicData = false;
    }

    return Chimera::CommonStatusCodes::OK;
  }

  Chimera::Status_t DoubleBuffer::push( const uint8_t *const buffer, const size_t len )
  {
    using namespace Chimera::Hardware;
    using namespace Chimera::Threading;

    auto error = Chimera::CommonStatusCodes::LOCKED;

    if ( !buffer )
    {
      return Chimera::CommonStatusCodes::INVAL_FUNC_PARAM;
    }

    if ( LockGuard(*this).lock() )
    {
      error = Chimera::CommonStatusCodes::OK;
      size_t bytesWritten = 0;

      while ( !pCircularBuffer->full() && ( bytesWritten < len ) )
      {
        pCircularBuffer->push_back( buffer[ bytesWritten ] );
        bytesWritten++;
      }

      if ( bytesWritten != len )
      {
        error = Chimera::CommonStatusCodes::FULL;
      }
    }

    return error;
  }

  Chimera::Status_t DoubleBuffer::pop( uint8_t *const buffer, const size_t len )
  {
    using namespace Chimera::Hardware;
    using namespace Chimera::Threading;

    auto error = Chimera::CommonStatusCodes::LOCKED;

    if ( !buffer )
    {
      return Chimera::CommonStatusCodes::INVAL_FUNC_PARAM;
    }
    
    if ( LockGuard( *this ).lock() )
    {
      error = Chimera::CommonStatusCodes::OK;
      size_t bytesRead = 0;

      if ( pCircularBuffer )
      {
        while ( !pCircularBuffer->empty() && ( bytesRead < len ) )
        {
          buffer[ bytesRead ] = pCircularBuffer->front();
          pCircularBuffer->pop_front();
          bytesRead++;
        }
      }

      if ( bytesRead != len )
      {
        error = Chimera::CommonStatusCodes::EMPTY;
      }
    }

    return error;
  }

  Chimera::Status_t DoubleBuffer::flush()
  {
    using namespace Chimera::Threading;
    auto error = Chimera::CommonStatusCodes::LOCKED;

    if ( LockGuard(*this).lock() )
    {
      error = Chimera::CommonStatusCodes::OK;

      if ( pLinearBuffer )
      {
        memset( pLinearBuffer, 0, linearLength );
      }
      else
      {
        error = Chimera::CommonStatusCodes::FAIL;
      }

      if ( pCircularBuffer )
      {
        pCircularBuffer->clear();
      }
      else
      {
        error = Chimera::CommonStatusCodes::FAIL;
      }
    }

    return error;
  }

  boost::circular_buffer<uint8_t> *const DoubleBuffer::circularBuffer()
  {
    return pCircularBuffer;
  }

  bool DoubleBuffer::initialized()
  {
    return ( pLinearBuffer && pCircularBuffer );
  }

  size_t DoubleBuffer::transferInto( const size_t bytes )
  {
    using namespace Chimera::Threading;

    size_t bytesToCopy = 0;

    if ( LockGuard(*this).lock() )
    {
      bytesToCopy = pCircularBuffer->size();

      if ( bytesToCopy > linearLength )
      {
        bytesToCopy = linearLength;
      }

      if ( bytesToCopy > bytes )
      {
        bytesToCopy = bytes;
      }

      size_t copied = 0;

      while ( copied < bytesToCopy )
      {
        pLinearBuffer[ copied ] = pCircularBuffer->front();
        pCircularBuffer->pop_front();
        copied++;
      }
    }

    return bytesToCopy;
  }

  void DoubleBuffer::freeDynamicData()
  {
    if( !dynamicData )
    {
      return;
    }

    if( pLinearBuffer )
    {
      delete[] pLinearBuffer;
    }

    if( pCircularBuffer )
    {
      pCircularBuffer->~circular_buffer();
      delete pCircularBuffer;
    }

    dynamicData = false;
  }
}  // namespace Chimera::Buffer