/********************************************************************************
*   File Name:
*       main.cpp
*
*   Description:
*       Entry point for the Chimera test suite
*
*   2019 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/
#include <gtest/gtest.h>
#include "gtest_init.hpp"

using namespace ::testing;

int main(int argc, char **argv)
{
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
