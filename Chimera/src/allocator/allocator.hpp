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

#include <Chimera/common>


#if defined( USING_FREERTOS_THREADS )
void* malloc (size_t size);

void free (void* ptr);

#include "new"
void *operator new( size_t size );

void *operator new[]( size_t size );

void operator delete( void *p ) noexcept;

#endif /* USING_FREERTOS_THREADS */

#endif /* CHIMERA_ALLOCATOR_HPP*/
