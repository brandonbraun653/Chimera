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
  void insert_breakpoint()
  {
    __asm( "BKPT #0\n" );
  }
}  // namespace Chimera
#endif

/*------------------------------------------------
WIN32
------------------------------------------------*/
#if defined( WIN32 )
#define WIN32_LEAN_AND_MEAN
#define _X86_
#include "windows.h"
#include "debugapi.h"

namespace Chimera
{
  void insert_breakpoint()
  {
    DebugBreak();
  }
}  // namespace Chimera
#endif

/*------------------------------------------------
WIN64
------------------------------------------------*/
#if defined( WIN64 )
#define WIN32_LEAN_AND_MEAN
#define _X64_
#include "windows.h"
#include "debugapi.h"

namespace Chimera
{
  void insert_breakpoint()
  {
    DebugBreak();
  }
}  // namespace Chimera
#endif