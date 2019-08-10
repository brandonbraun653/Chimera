/********************************************************************************
 *   File Name:
 *       buffer_test.cpp
 *
 *   Description:
 *       Tests the Chimera buffer file
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <random>

/* Boost Includes */
#include <boost/circular_buffer.hpp>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/buffer.hpp>

/* GTest Includes */
#include <gtest/gtest.h>

using namespace Chimera::Buffer;

static constexpr size_t defaultLinearBufferSize = 100;

static std::vector<uint8_t> randomDataFactory( const size_t size )
{
  using namespace std;

  random_device rnd_device;
  mt19937 mersenne_engine{ rnd_device() };
  uniform_int_distribution<unsigned short> dist{ 0x00, 0xFF };

  auto gen = [&dist, &mersenne_engine]() { return dist( mersenne_engine ); };

  vector<uint8_t> randomData( size );
  generate( randomData.begin(), randomData.end(), gen );

  return randomData;
}

namespace BoundaryChecks
{
  TEST( BoundaryChecks, NotInitialized )
  {
    PeripheralBuffer buffer;
    ASSERT_EQ( buffer.initialized(), false );
  }

  TEST( BoundaryChecks, AssignDynamic_NullInput )
  {
    PeripheralBuffer buffer;

    auto result      = buffer.assign( 0, 10 );
    auto initialized = buffer.initialized();
    ASSERT_EQ( result, Chimera::CommonStatusCodes::INVAL_FUNC_PARAM );
    ASSERT_EQ( initialized, false );

    result      = buffer.assign( 10, 0 );
    initialized = buffer.initialized();
    ASSERT_EQ( result, Chimera::CommonStatusCodes::INVAL_FUNC_PARAM );
    ASSERT_EQ( initialized, false );
  }

  TEST( BoundaryChecks, AssignStatic_NullInput )
  {
    PeripheralBuffer buffer;
    boost::circular_buffer<uint8_t> cbuf;
    std::array<uint8_t, 100> lbuf;

    /*------------------------------------------------
    All null inputs
    ------------------------------------------------*/
    auto result      = buffer.assign( nullptr, nullptr, 0 );
    auto initialized = buffer.initialized();

    ASSERT_EQ( result, Chimera::CommonStatusCodes::INVAL_FUNC_PARAM );
    ASSERT_EQ( initialized, false );

    /*------------------------------------------------
    Valid circular buffer
    ------------------------------------------------*/
    result      = buffer.assign( &cbuf, nullptr, 0 );
    initialized = buffer.initialized();

    ASSERT_EQ( result, Chimera::CommonStatusCodes::INVAL_FUNC_PARAM );
    ASSERT_EQ( initialized, false );

    /*------------------------------------------------
    Valid linear buffer
    ------------------------------------------------*/
    result      = buffer.assign( nullptr, lbuf.data(), lbuf.size() );
    initialized = buffer.initialized();

    ASSERT_EQ( result, Chimera::CommonStatusCodes::INVAL_FUNC_PARAM );
    ASSERT_EQ( initialized, false );

    /*------------------------------------------------
    Partially valid linear buffer
    ------------------------------------------------*/
    result      = buffer.assign( &cbuf, lbuf.data(), 0 );
    initialized = buffer.initialized();

    ASSERT_EQ( result, Chimera::CommonStatusCodes::INVAL_FUNC_PARAM );
    ASSERT_EQ( initialized, false );
  }

  TEST( BoundaryChecks, Push_NullInput )
  {
    /*------------------------------------------------
    Initialize
    ------------------------------------------------*/
    PeripheralBuffer buffer;
    uint8_t tmp;
    size_t tmpSize        = sizeof( tmp );
    size_t actuallyPushed = 10;

    /*------------------------------------------------
    Bad buffer inputs
    ------------------------------------------------*/
    auto result = buffer.push( nullptr, tmpSize, actuallyPushed );
    ASSERT_EQ( result, Chimera::CommonStatusCodes::INVAL_FUNC_PARAM );

    result = buffer.push( &tmp, 0, actuallyPushed );
    ASSERT_EQ( result, Chimera::CommonStatusCodes::INVAL_FUNC_PARAM );

    /*------------------------------------------------
    Circular buffer not initialized
    ------------------------------------------------*/
    result = buffer.push( &tmp, tmpSize, actuallyPushed );
    ASSERT_EQ( result, Chimera::CommonStatusCodes::NOT_INITIALIZED );
  }

  TEST( BoundaryChecks, Push_LockedDriver )
  {
    /*------------------------------------------------
    Initialize
    ------------------------------------------------*/
    PeripheralBuffer buffer;
    uint8_t tmp;
    size_t tmpSize        = sizeof( tmp );
    size_t actuallyPushed = 10;

    buffer.assign( 10, 10 );

    ASSERT_EQ( buffer.lock( 10 ), Chimera::CommonStatusCodes::OK );

    /*------------------------------------------------
    Call while locked
    ------------------------------------------------*/
    auto result = buffer.push( &tmp, tmpSize, actuallyPushed );
    ASSERT_EQ( result, Chimera::CommonStatusCodes::LOCKED );
  }

  TEST( BoundaryChecks, Pop_NullInput )
  {
    /*------------------------------------------------
    Initialize
    ------------------------------------------------*/
    PeripheralBuffer buffer;
    uint8_t tmp;
    size_t tmpSize        = sizeof( tmp );
    size_t actuallyPopped = 10;

    /*------------------------------------------------
    Bad buffer inputs
    ------------------------------------------------*/
    auto result = buffer.pop( nullptr, tmpSize, actuallyPopped );
    ASSERT_EQ( result, Chimera::CommonStatusCodes::INVAL_FUNC_PARAM );

    result = buffer.pop( &tmp, 0, actuallyPopped );
    ASSERT_EQ( result, Chimera::CommonStatusCodes::INVAL_FUNC_PARAM );

    /*------------------------------------------------
    Circular buffer not initialized
    ------------------------------------------------*/
    result = buffer.pop( &tmp, tmpSize, actuallyPopped );
    ASSERT_EQ( result, Chimera::CommonStatusCodes::NOT_INITIALIZED );
  }

  TEST( BoundaryChecks, Pop_LockedDriver )
  {
    /*------------------------------------------------
    Initialize
    ------------------------------------------------*/
    PeripheralBuffer buffer;
    uint8_t tmp;
    size_t tmpSize        = sizeof( tmp );
    size_t actuallyPopped = 10;

    buffer.assign( 10, 10 );

    ASSERT_EQ( buffer.lock( 10 ), Chimera::CommonStatusCodes::OK );

    /*------------------------------------------------
    Call while locked
    ------------------------------------------------*/
    auto result = buffer.pop( &tmp, tmpSize, actuallyPopped );
    ASSERT_EQ( result, Chimera::CommonStatusCodes::LOCKED );
  }

  TEST( BoundaryChecks, Flush_NotInitialized )
  {
    PeripheralBuffer buffer;

    auto result = buffer.flush();
    ASSERT_EQ( result, Chimera::CommonStatusCodes::NOT_INITIALIZED );
  }

  TEST( BoundaryChecks, Flush_LockedDriver )
  {
    PeripheralBuffer buffer;

    buffer.assign( 10, 10 );
    buffer.lock( 100 );

    auto result = buffer.flush();
    ASSERT_EQ( result, Chimera::CommonStatusCodes::LOCKED );
  }

  TEST( BoundaryChecks, TransferInto_NotInitialized )
  {
    PeripheralBuffer buffer;
    size_t tmp  = 0;
    auto result = buffer.transferInto( 10, tmp );
    ASSERT_EQ( result, Chimera::CommonStatusCodes::NOT_INITIALIZED );
  }

  TEST( BoundaryChecks, TransferInto_LockedDriver )
  {
    PeripheralBuffer buffer;
    size_t tmp = 0;

    buffer.assign( 10, 10 );
    buffer.lock( 100 );

    auto result = buffer.transferInto( 10, tmp );
    ASSERT_EQ( result, Chimera::CommonStatusCodes::LOCKED );
  }

  TEST( BoundaryChecks, TransferOutOf_NotInitialized )
  {
    PeripheralBuffer buffer;
    size_t tmp  = 0;
    auto result = buffer.transferOutOf( 10, tmp );
    ASSERT_EQ( result, Chimera::CommonStatusCodes::NOT_INITIALIZED );
  }

  TEST( BoundaryChecks, TransferOutOf_LockedDriver )
  {
    PeripheralBuffer buffer;
    size_t tmp = 0;

    buffer.assign( 10, 10 );
    buffer.lock( 100 );

    auto result = buffer.transferOutOf( 10, tmp );
    ASSERT_EQ( result, Chimera::CommonStatusCodes::LOCKED );
  }
}  // namespace BoundaryChecks

namespace NormalOperation
{
  /*------------------------------------------------
  PeripheralBuffer::assign()
  ------------------------------------------------*/
  TEST( NormalOperation, AssignDynamic )
  {
    PeripheralBuffer buffer;

    const size_t circularBufferSize = static_cast<size_t>( rand() % 100 + 1 );
    const size_t linearBufferSize   = static_cast<size_t>( rand() % 100 + 1 );

    auto result = buffer.assign( circularBufferSize, linearBufferSize );
    ASSERT_EQ( result, Chimera::CommonStatusCodes::OK );
  }

  TEST( NormalOperation, AssignStatic )
  {
    PeripheralBuffer buffer;

    const size_t linearBufferSize   = static_cast<size_t>( rand() % 100 + 1 );

    boost::circular_buffer<uint8_t> cbuf;
    uint8_t *lbuf = new uint8_t[ linearBufferSize ];

    auto result = buffer.assign( &cbuf, lbuf, linearBufferSize );
    ASSERT_EQ( result, Chimera::CommonStatusCodes::OK );
  }

  /*------------------------------------------------
  PeripheralBuffer::push()
  PeripheralBuffer::pop()
  ------------------------------------------------*/
  TEST( NormalOperation, PushPopSymmetrical )
  {
    /*------------------------------------------------
    Initialize
    ------------------------------------------------*/
    PeripheralBuffer buffer;

    const size_t circularBufferSize = static_cast<size_t>( rand() % 100 + 1 );
    const size_t linearBufferSize   = circularBufferSize;

    /*------------------------------------------------
    Input managed memory
    ------------------------------------------------*/
    buffer.assign( circularBufferSize, linearBufferSize );
    ASSERT_EQ( buffer.initialized(), true );

    /*------------------------------------------------
    Create memory for IO
    ------------------------------------------------*/
    auto randomData = randomDataFactory( circularBufferSize );
    std::vector<uint8_t> outputData( circularBufferSize );

    /*------------------------------------------------
    Run the test
    ------------------------------------------------*/
    size_t actuallyPushed = ~circularBufferSize;
    auto pushResult       = buffer.push( randomData.data(), randomData.size(), actuallyPushed );

    ASSERT_EQ( actuallyPushed, circularBufferSize );
    ASSERT_EQ( pushResult, Chimera::CommonStatusCodes::OK );

    size_t actuallyPopped = ~circularBufferSize;
    auto popResult        = buffer.pop( outputData.data(), outputData.size(), actuallyPopped );

    ASSERT_EQ( actuallyPopped, circularBufferSize );
    ASSERT_EQ( popResult, Chimera::CommonStatusCodes::OK );

    ASSERT_EQ( outputData, randomData );
  }

  TEST( NormalOperation, PushFull )
  {
    /*------------------------------------------------
    Initialize
    ------------------------------------------------*/
    PeripheralBuffer buffer;

    const size_t circularBufferSize = 5;
    auto randomData                 = randomDataFactory( circularBufferSize + 1 );

    buffer.assign( circularBufferSize, 1 );

    /*------------------------------------------------
    Call
    ------------------------------------------------*/
    size_t actuallyPushed = 0;
    auto result           = buffer.push( randomData.data(), randomData.size(), actuallyPushed );

    /*------------------------------------------------
    Verify
    ------------------------------------------------*/
    ASSERT_EQ( result, Chimera::CommonStatusCodes::FULL );
    ASSERT_EQ( actuallyPushed, circularBufferSize );
  }

  TEST( NormalOperation, PopEmpty )
  {
    /*------------------------------------------------
    Initialize
    ------------------------------------------------*/
    PeripheralBuffer buffer;

    const size_t circularBufferSize = 5;
    size_t actuallyPushed           = 0;
    size_t actuallyPopped           = 0;

    auto randomData = randomDataFactory( circularBufferSize );
    std::array<uint8_t, circularBufferSize * 2> outputData;

    outputData.fill( 0 );
    buffer.assign( circularBufferSize, 1 );
    buffer.push( randomData.data(), randomData.size(), actuallyPushed );

    /*------------------------------------------------
    Call
    ------------------------------------------------*/
    auto result = buffer.pop( outputData.data(), outputData.size(), actuallyPopped );

    /*------------------------------------------------
    Verify
    ------------------------------------------------*/
    ASSERT_EQ( result, Chimera::CommonStatusCodes::EMPTY );
    ASSERT_EQ( actuallyPopped, circularBufferSize );
  }

  /*------------------------------------------------
  PeripheralBuffer::flush()
  ------------------------------------------------*/
  TEST( NormalOperation, FlushTheToilet )
  {
    PeripheralBuffer buffer;

    buffer.assign( 10, 10 );

    auto result = buffer.flush();
    ASSERT_EQ( result, Chimera::CommonStatusCodes::OK );
  }

  /*------------------------------------------------
  PeripheralBuffer::circularBuffer()
  PeripheralBuffer::linearBuffer()
  ------------------------------------------------*/
  TEST( NormalOperation, BufferAccess )
  {
    /*------------------------------------------------
    Initialize
    ------------------------------------------------*/
    PeripheralBuffer buffer;
    const size_t linearBufferSize   = static_cast<size_t>( rand() % 100 + 1 );

    boost::circular_buffer<uint8_t> cbuf;
    uint8_t *lbuf = new uint8_t[ linearBufferSize ];

    auto result = buffer.assign( &cbuf, lbuf, linearBufferSize );
    ASSERT_EQ( result, Chimera::CommonStatusCodes::OK );

    /*------------------------------------------------
    Call
    ------------------------------------------------*/
    auto internalCBuf = buffer.circularBuffer();
    auto internalLBuf = buffer.linearBuffer();

    /*------------------------------------------------
    Verify
    ------------------------------------------------*/
    ASSERT_EQ( reinterpret_cast<std::uintptr_t>( internalCBuf ), reinterpret_cast<std::uintptr_t>( &cbuf ) );
    ASSERT_EQ( reinterpret_cast<std::uintptr_t>( internalLBuf ), reinterpret_cast<std::uintptr_t>( lbuf ) );
  }

  /*------------------------------------------------
  PeripheralBuffer::transferInto()
  ------------------------------------------------*/
  TEST( NormalOperation, CircularToLinear_UndersizedCopy )
  {
    /*------------------------------------------------
    Initialize
    ------------------------------------------------*/
    PeripheralBuffer buffer;
    size_t pushedBytes   = 0;
    size_t transferBytes = 0;

    const size_t circularBufferSize = static_cast<size_t>( rand() % 100 + 10 );
    const size_t linearBufferSize   = circularBufferSize;

    auto randomData = randomDataFactory( circularBufferSize );

    buffer.assign( circularBufferSize, linearBufferSize );
    buffer.push( randomData.data(), randomData.size(), pushedBytes );

    /*------------------------------------------------
    Call
    ------------------------------------------------*/
    size_t txfrSize = linearBufferSize / 2u;
    auto result     = buffer.transferInto( txfrSize, transferBytes );

    /*------------------------------------------------
    Verify
    ------------------------------------------------*/
    ASSERT_EQ( result, Chimera::CommonStatusCodes::OK );
    ASSERT_EQ( txfrSize, transferBytes );
    ASSERT_TRUE( txfrSize < pushedBytes );
  }

  TEST( NormalOperation, CircularToLinear_UndersizedBuffer )
  {
    /*------------------------------------------------
    Initialize
    ------------------------------------------------*/
    PeripheralBuffer buffer;
    size_t pushedBytes   = 0;
    size_t transferBytes = 0;

    const size_t circularBufferSize = static_cast<size_t>( rand() % 100 + 10 );
    const size_t linearBufferSize   = circularBufferSize / 2;

    auto randomData = randomDataFactory( circularBufferSize );

    buffer.assign( circularBufferSize, linearBufferSize );
    buffer.push( randomData.data(), randomData.size(), pushedBytes );

    /*------------------------------------------------
    Call
    ------------------------------------------------*/
    size_t txfrSize = linearBufferSize * 2u;
    auto result     = buffer.transferInto( txfrSize, transferBytes );

    /*------------------------------------------------
    Verify
    ------------------------------------------------*/
    ASSERT_EQ( result, Chimera::CommonStatusCodes::FULL );
    ASSERT_EQ( transferBytes, linearBufferSize );
    ASSERT_TRUE( transferBytes < pushedBytes );
  }

  /*------------------------------------------------
  PeripheralBuffer::transferOutOf()
  ------------------------------------------------*/
  TEST( NormalOperation, LinearToCircular_UndersizedCopy )
  {
    /*------------------------------------------------
    Initialize
    ------------------------------------------------*/
    PeripheralBuffer buffer;
    size_t transferBytes = 0;

    const size_t circularBufferSize = static_cast<size_t>( rand() % 100 + 10 );
    const size_t linearBufferSize   = circularBufferSize;

    const auto randomData = randomDataFactory( circularBufferSize );

    buffer.assign( circularBufferSize, linearBufferSize );
    memcpy( buffer.linearBuffer(), randomData.data(), randomData.size() );

    /*------------------------------------------------
    Call
    ------------------------------------------------*/
    size_t txfrSize = linearBufferSize / 2u;
    auto result     = buffer.transferOutOf( txfrSize, transferBytes );

    /*------------------------------------------------
    Verify
    ------------------------------------------------*/
    ASSERT_EQ( result, Chimera::CommonStatusCodes::OK );
    ASSERT_EQ( txfrSize, transferBytes );
    ASSERT_TRUE( txfrSize < randomData.size() );
  }

  TEST( NormalOperation, LinearToCircular_MaxedCircularCopy )
  {
    /*------------------------------------------------
    Initialize
    ------------------------------------------------*/
    PeripheralBuffer buffer;
    size_t transferBytes = 0;

    const size_t circularBufferSize = static_cast<size_t>( rand() % 100 + 10 );
    const size_t linearBufferSize   = circularBufferSize * 2;

    const auto randomData = randomDataFactory( circularBufferSize );

    buffer.assign( circularBufferSize, linearBufferSize );
    memcpy( buffer.linearBuffer(), randomData.data(), randomData.size() );

    /*------------------------------------------------
    Call
    ------------------------------------------------*/
    size_t txfrSize = linearBufferSize;
    auto result     = buffer.transferOutOf( txfrSize, transferBytes );

    /*------------------------------------------------
    Verify
    ------------------------------------------------*/
    ASSERT_EQ( result, Chimera::CommonStatusCodes::FULL );
    ASSERT_EQ( transferBytes, circularBufferSize );
    ASSERT_TRUE( linearBufferSize > circularBufferSize );
  }

  TEST( NormalOperation, LinearToCircular_OversizedLinearCopy )
  {
    /*------------------------------------------------
    Initialize
    ------------------------------------------------*/
    PeripheralBuffer buffer;
    size_t transferBytes = 0;

    const size_t circularBufferSize = static_cast<size_t>( rand() % 100 + 10 );
    const size_t linearBufferSize   = circularBufferSize;

    const auto randomData = randomDataFactory( circularBufferSize );

    buffer.assign( circularBufferSize, linearBufferSize );
    memcpy( buffer.linearBuffer(), randomData.data(), randomData.size() );

    /*------------------------------------------------
    Call
    ------------------------------------------------*/
    size_t txfrSize = linearBufferSize * 2;
    auto result     = buffer.transferOutOf( txfrSize, transferBytes );

    /*------------------------------------------------
    Verify
    ------------------------------------------------*/
    ASSERT_EQ( result, Chimera::CommonStatusCodes::OK );
    ASSERT_EQ( transferBytes, linearBufferSize );
    ASSERT_TRUE( txfrSize > linearBufferSize );
  }

  /*------------------------------------------------
  Data validation tests
  ------------------------------------------------*/
  TEST( NormalOperation, Push_TransferInto_TransferOutOf_Pop )
  {
    /*------------------------------------------------
    Initialize
    ------------------------------------------------*/
    const size_t bufferSizes = 100;
    size_t actuallyPushed = 0;
    size_t actuallyPopped = 0;
    size_t tmp1 = 0;
    size_t tmp2 = 0;

    PeripheralBuffer buffer;
    std::vector<uint8_t> inputData = randomDataFactory( bufferSizes );
    std::array<uint8_t, bufferSizes> outputData;

    outputData.fill(0);

    buffer.assign(bufferSizes, bufferSizes);

    /*------------------------------------------------
    Call
    ------------------------------------------------*/
    buffer.push(inputData.data(), bufferSizes, actuallyPushed );
    buffer.transferInto(bufferSizes, tmp1);
    buffer.transferOutOf(bufferSizes, tmp2);
    buffer.pop(outputData.data(), bufferSizes, actuallyPopped);

    /*------------------------------------------------
    Verify
    ------------------------------------------------*/
    ASSERT_EQ( actuallyPushed, bufferSizes );
    ASSERT_EQ( actuallyPopped, bufferSizes );
    ASSERT_EQ( tmp1, bufferSizes );
    ASSERT_EQ( tmp2, bufferSizes );
    ASSERT_TRUE( memcmp(inputData.data(), outputData.data(), bufferSizes) == 0 );
  }
}  // namespace NormalOperation