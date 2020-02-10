/********************************************************************************
*  File Name:
*    container_test.cpp
*
*  Description:
*    Test the Chimera Container module
*
*  2019 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/

/* STL Includes */
#include <cstdlib>


/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/container.hpp>

/* GTest Includes */
#include <gtest/gtest.h>

using namespace Chimera::Container;

static constexpr size_t sdSize = 3;

static const LightFlatMap<std::uintptr_t, size_t, sdSize> staticData{ 
  { 0, 0 }, { 1, 1 }, { 2, 2 } 
  };

namespace BoundaryChecks
{
  TEST( BoundaryChecks, InvalidKey )
  {
    constexpr size_t size = 5u;
    auto testVar          = LightFlatMap<uint8_t, uint8_t, size>{ { 10, 10 }, { 3, 4 }, { 10, 10 }, { 3, 4 }, { 10, 10 } };

    auto tmp = testVar.find(35);
    ASSERT_EQ( tmp, nullptr );
  }
}

namespace NormalOperation
{
  TEST( NormalOperation, Sizing )
  {
    constexpr size_t size = 1u;
    auto testVar = LightFlatMap<uint8_t, uint8_t, size>{{10,10}};

    ASSERT_EQ( size, testVar.size() );
  }

  TEST( NormalOperation, StaticConstData )
  {
    ASSERT_EQ( sdSize, staticData.size() );
  }

  TEST( NormalOperation, FindValidReference )
  {
    constexpr size_t size = 5u;
    auto testVar          = LightFlatMap<uint8_t, uint8_t, size>{ { 10, 10 }, { 3, 4 }, { 10, 10 }, { 3, 4 }, { 10, 10 } };

    auto tmp = testVar.find( static_cast<uint8_t>( 10 ) );
    ASSERT_NE( tmp, nullptr );
  }

  TEST( NormalOperation, ValueAssociatedWithKey )
  {
    constexpr size_t size = 5u;
    auto testVar          = LightFlatMap<uint8_t, uint8_t, size>{ { 10, 10 }, { 3, 4 }, { 10, 10 }, { 3, 4 }, { 10, 10 } };

    auto tmp = testVar[ 10 ];
    ASSERT_EQ( tmp, 10 );
  }
}