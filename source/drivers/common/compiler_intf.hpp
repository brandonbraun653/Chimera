/********************************************************************************
 *  File Name:
 *    compiler_intf.hpp
 *
 *  Description:
 *    Contains macros for various functionality that must be portable between
 *    compilers.
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

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

/*------------------------------------------------
GCC
------------------------------------------------*/
#if defined( __GNUC__ )

#if !defined( WEAKDECL )
#define WEAKDECL __attribute__( ( weak ) )
#endif

#ifdef __GNUC__
#define UNUSED( x ) UNUSED_##x __attribute__( ( __unused__ ) )
#else
#define UNUSED( x ) UNUSED_##x
#endif

#ifdef __GNUC__
#define UNUSED_FUNCTION( x ) __attribute__( ( __unused__ ) ) UNUSED_##x
#else
#define UNUSED_FUNCTION( x ) UNUSED_##x
#endif

#endif /* __GNUC__ */

#endif  /* CHIMERA_COMPILER_INTERFACE_HPP */
