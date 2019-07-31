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

TEST( BoundaryChecks, NullInput )
{
  PeripheralBuffer buffer;

  ASSERT_EQ( buffer.initialized(), false );
}