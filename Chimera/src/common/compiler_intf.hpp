/********************************************************************************
 *   File Name:
 *    compiler_intf.hpp
 *
 *   Description:
 *    Contains macros for various functionality that must be portable between 
 *    compilers.
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_COMPILER_INTERFACE_HPP
#define CHIMERA_COMPILER_INTERFACE_HPP

namespace Chimera::Compiler
{
/*------------------------------------------------
GCC 
------------------------------------------------*/
#if defined( __GNUC__ )

#if !defined( WEAKDECL )
#define WEAKDECL __attribute__((weak))
#endif

#if !defined( CHIMERA_INSERT_BREAKPOINT )
#define CHIMERA_INSERT_BREAKPOINT __asm( "BKPT #0\n" )
#endif

#endif /* __GNUC__ */

/*------------------------------------------------
WIN32
------------------------------------------------*/
#if defined( WIN32 )
//#define WIN32_LEAN_AND_MEAN
//#define _X32_
//#define _X86_
//#include "debugapi.h"


#define WEAKDECL
//#define CHIMERA_INSERT_BREAKPOINT DebugBreak()

#endif /* WIN32 */

/*------------------------------------------------
WIN64
------------------------------------------------*/
#if defined( WIN64 )
#define WIN32_LEAN_AND_MEAN
#define _X64_
#include "debugapi.h"


#define WEAKDECL
#define CHIMERA_INSERT_BREAKPOINT DebugBreak()

#endif /* WIN64 */


// /*------------------------------------------------
// Weak function declarations 
// ------------------------------------------------*/
// #if defined( __GNUC__ )



// #elif defined( WIN32 ) || defined( WIN64 )

// #define WEAKDECL

// #else 

// #define WEAKDECL

// #endif /* */

}

#endif  // ! CHIMERA_COMPILER_INTERFACE_HPP
