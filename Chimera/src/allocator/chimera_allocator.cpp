/********************************************************************************
 *  File Name:
 *	  chimera_allocator.hpp
 *
 *  Description:
 *	  Provides overloads for common memory allocators and deleters
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* STL Includes */
#include <cstdlib>

/* Chimera Includes */
#include <Chimera/src/allocator/allocator.hpp>
#include <Chimera/thread>

#if defined( USING_FREERTOS ) && ( CHIMERA_CFG_FREERTOS == 1 )

#include "FreeRTOS.h"
#include "portable.h"

#if !defined( SIM )
void *malloc( size_t size )
{
  return pvPortMalloc( size );
}

void free( void *ptr )
{
  vPortFree( ptr );
}
#endif /* !SIM */

/*------------------------------------------------
Only overload these operators if we aren't using the MSVC runtime.
I was unsuccessful in figuring out a thread safe way to mix and match
FreeRTOS threads and std::threads without corrupting memory.
------------------------------------------------*/
#if !defined( WIN32 ) && !defined( WIN64 )
void *operator new( size_t size )
{
  return pvPortMalloc( size );
}

void *operator new[]( size_t size )
{
  return pvPortMalloc( size );
}

void operator delete( void *p ) noexcept
{
  vPortFree( p );
}
#endif /* !WIN32 && !WIN64 */

#endif /* CHIMERA_CFG_FREERTOS */