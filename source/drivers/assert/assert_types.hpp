/********************************************************************************
 *  File Name:
 *    assert_types.hpp
 *
 *  Description:
 *    Types and definitions for the Assert module
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_ASSERT_TYPES_HPP
#define CHIMERA_ASSERT_TYPES_HPP

/* STL Includes */
#include <cstdint>

namespace Chimera::Assert
{
  /*-------------------------------------------------------------------------------
  Enumerations
  -------------------------------------------------------------------------------*/
  enum AssertionFlags
  {
    ASSERT_POWER_OFF,
    ASSERT_CONTINUE,
  };
}  // namespace Chimera::Assert

#endif  /* !CHIMERA_ASSERT_TYPES_HPP */
