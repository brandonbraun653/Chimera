/******************************************************************************
 *  File Name:
 *    compiler_intf.hpp
 *
 *  Description:
 *    Contains macros for various functionality that must be portable between
 *    compilers.
 *
 *  2019-2022 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_COMPILER_INTERFACE_HPP
#define CHIMERA_COMPILER_INTERFACE_HPP

namespace Chimera
{
  /**
   * Inserts a breakpoint into the program at the call site.
   */
  void insert_debug_breakpoint();
}  // namespace Chimera

/*-----------------------------------------------------------------------------
GCC
-----------------------------------------------------------------------------*/
#if defined( __GNUC__ )
#include <sys/cdefs.h>

#define __weak __attribute__( ( weak ) )
#define __packed_struct struct __attribute__( ( packed ) )
#define UNUSED( x ) UNUSED_##x __attribute__( ( __unused__ ) )
#define UNUSED_FUNCTION( x ) __attribute__( ( __unused__ ) ) UNUSED_##x

/*-----------------------------------------------------------------------------
Unknown Compiler
-----------------------------------------------------------------------------*/
#else

#define __weak
#define __packed
#define UNUSED( x ) UNUSED_##x
#define UNUSED_FUNCTION( x ) UNUSED_##x

#endif /* __GNUC__ */

#endif  /* CHIMERA_COMPILER_INTERFACE_HPP */
