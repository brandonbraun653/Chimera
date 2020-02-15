/********************************************************************************
 *  File Name:
 *	  allocator.hpp
 *
 *  Description:
 *	  Provides overloads for common memory allocators and deleters
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_ALLOCATOR_HPP
#define CHIMERA_ALLOCATOR_HPP

#include "chimeraConfig.hpp"
#include <Chimera/common>

/*------------------------------------------------
Redirect the new/delete operators into the FreeRTOS
memory management functions. Without this, all hell
will break loose.
------------------------------------------------*/
#if defined( CHIMERA_CFG_FREERTOS ) && ( CHIMERA_CFG_FREERTOS == 1 )

#if !defined( CHIMERA_SIMULATOR )
void* malloc (size_t size);

void free (void* ptr);
#endif /* !CHIMERA_SIMULATOR */

#include "new"
void *operator new( size_t size );

void *operator new[]( size_t size );

void operator delete( void *p ) noexcept;

#endif /* CHIMERA_CFG_FREERTOS */
#endif /* CHIMERA_ALLOCATOR_HPP*/
