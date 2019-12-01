/********************************************************************************
 * File Name:
 *	  chimera_allocator.hpp
 *
 * Description:
 *	  Provides overloads for common memory allocators and deleters
 *
 * 2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */
#include <cstdlib>

/* Chimera Includes */
#include <Chimera/allocator.hpp>

#if defined( CHIMERA_CFG_FREERTOS ) && (CHIMERA_CFG_FREERTOS == 1 )

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


void *operator new( size_t size )
{
  void *p = pvPortMalloc( size );

#ifdef __EXCEPTIONS
  if ( p == 0 )                // did pvPortMalloc succeed?
    throw std::bad_alloc();    // ANSI/ISO compliant behavior
#endif
  return p;
}

void *operator new[]( size_t size )
{
  void *p = pvPortMalloc( size );

#ifdef __EXCEPTIONS
  if ( p == 0 )                // did pvPortMalloc succeed?
    throw std::bad_alloc();    // ANSI/ISO compliant behavior
#endif
  return p;
}

void operator delete( void *p ) noexcept
{
  vPortFree( p );
}

#endif /* CHIMERA_CFG_FREERTOS */