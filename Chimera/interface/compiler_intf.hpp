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
Weak function declarations 
------------------------------------------------*/
#if defined( __GNUC__ )

#define WEAKDECL __attribute__((weak))

#elif defined( WIN32 ) || defined( WIN64 )

#define WEAKDECL

#else 

#define WEAKDECL

#endif /* */

/*------------------------------------------------
Inserting breakpoints
------------------------------------------------*/
#if defined( __GNUC__ )

#elif defined( WIN32 ) || defined( WIN64 )


#define INSERT_BREAKPOINT (  )

#else

#define INSERT_BREAKPOINT

#endif /* */

}

#endif  // ! CHIMERA_COMPILER_INTERFACE_HPP
