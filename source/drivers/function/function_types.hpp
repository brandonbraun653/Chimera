/********************************************************************************
 *  File Name:
 *    function_types.hpp
 *
 *  Description:
 *    Chimera Function Type Definitions
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_FUNCTION_TYPES_HPP
#define CHIMERA_FUNCTION_TYPES_HPP

/* STL Includes */
#include <cstdint>

/* ETL Includes */
#include <etl/delegate.h>

namespace Chimera::Function
{
  /**
   *  Signature that specifies a function which accepts no
   *  arguments and returns nothing.
   */
  using Opaque = etl::delegate<void(void)>;

  /**
   *  Signature that returns nothing but can accept anything in
   *  the function arguments.
   */
  using vGeneric = etl::delegate<void(void*)>;

  /**
   *  Signature that returns some kind of boolean value. Used when
   *  the function pointer needs to inform the caller of some result.
   */
  using PassFail = bool ( * )( void );

  /**
   *  Signature to evaluate arguments and return a pass/fail result.
   */
  using PassFailGeneric = bool ( * )( void * );


}  // namespace Chimera::Function

#endif  /* !CHIMERA_FUNCTION_TYPES_HPP */
