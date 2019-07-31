/********************************************************************************
 *   File Name:
 *       buffer_test.cpp
 *
 *   Description:
 *       Tests the Chimera buffer file
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#include <iostream>

#include <Chimera/chimera.hpp>
#include <Chimera/buffer.hpp>

#include <gtest/gtest.h>

using namespace Chimera::Buffer;

namespace BufferTests
{
  TEST( BoundaryChecks, NotInitialized )
  {
    PeripheralBuffer buffer;
    ASSERT_EQ( buffer.initialized(), false );
  }

  TEST( BoundaryChecks, NullInputDynamic )
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
}
