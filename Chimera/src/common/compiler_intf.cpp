/********************************************************************************
 *  File Name:
 *    compiler_intf.cpp
 *
 *  Description:
 *    Compiler specific implementations of useful functions
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Chimera Includes */
#include <Chimera/src/common/compiler_intf.hpp>

/*------------------------------------------------
GCC
------------------------------------------------*/
#if defined( __GNUC__ )
namespace Chimera
{
   void insert_debug_breakpoint()
  {
    #if defined( DEBUG )
    __asm( "BKPT #0\n" );
    #endif 
  }
}  // namespace Chimera

#elif defined( WIN32 )

#define WIN32_LEAN_AND_MEAN
#define _X86_
#include "windows.h"
#include "debugapi.h"

namespace Chimera
{
   void insert_debug_breakpoint()
  {
    #if defined( DEBUG )
    DebugBreak();
    #endif 
  }
}  // namespace Chimera

#elif defined( WIN64 )

#define WIN32_LEAN_AND_MEAN
#define _X64_
#include "windows.h"
#include "debugapi.h"

namespace Chimera
{
   void insert_debug_breakpoint()
  {
    #if defined( DEBUG )
    DebugBreak();
    #endif 
  }
}  // namespace Chimera

#else
#pragma message( "Unknown build environment" )
namespace Chimera
{
   void insert_debug_breakpoint()
  {

  }
}
#endif