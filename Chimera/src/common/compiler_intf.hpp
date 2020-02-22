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
  void insert_breakpoint();
}  // namespace Chimera

/*------------------------------------------------
GCC
------------------------------------------------*/
#if defined( __GNUC__ )

#if !defined( WEAKDECL )
#define WEAKDECL __attribute__( ( weak ) )
#endif

#endif /* __GNUC__ */

/*------------------------------------------------
WIN32
------------------------------------------------*/
#if defined( WIN32 )
#ifndef WEAKDECL
#define WEAKDECL
#endif

#endif /* WIN32 */

/*------------------------------------------------
WIN64
------------------------------------------------*/
#if defined( WIN64 )
#ifndef WEAKDECL
#define WEAKDECL
#endif

#endif /* WIN64 */

#endif  // ! CHIMERA_COMPILER_INTERFACE_HPP
