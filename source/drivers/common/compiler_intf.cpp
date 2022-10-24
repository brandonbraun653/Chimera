/********************************************************************************
 *  File Name:
 *    compiler_intf.cpp
 *
 *  Description:
 *    Compiler specific implementations of useful functions
 *
 *  2020-2021 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* STL Includes */
#include <signal.h>

/* Chimera Includes */
#include <Chimera/source/drivers/common/compiler_intf.hpp>

/*-------------------------------------------------------------------------------
GCC
-------------------------------------------------------------------------------*/
#if defined( __GNUC__ )
namespace Chimera
{
  void insert_debug_breakpoint()
  {
#if !defined( DISABLE_DEBUG_BREAK )
  #if defined( EMBEDDED )
    asm volatile( "BKPT #0\n" );
  #else
    raise( SIGTRAP );
#endif  /* EMBEDDED */
#endif  /* !DISABLE_DEBUG_BREAK */
  }
}  // namespace Chimera

/*-------------------------------------------------------------------------------
WIN32
-------------------------------------------------------------------------------*/
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

/*-------------------------------------------------------------------------------
WIN64
-------------------------------------------------------------------------------*/
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

/*-------------------------------------------------------------------------------
UNKNOWN
-------------------------------------------------------------------------------*/
#else
#pragma message( "Unknown build environment" )
namespace Chimera
{
  void insert_debug_breakpoint()
  {
  }
}  // namespace Chimera
#endif
