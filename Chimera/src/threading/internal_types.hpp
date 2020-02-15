/********************************************************************************
*  File Name:
*    internal_types.hpp
*
*  Description:
*    Includes OS specific types for threading primitives    
*
*  2020 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/

#pragma once
#ifndef CHIMERA_THREADING_OS_TYPING_DETAIL_HPP
#define CHIMERA_THREADING_OS_TYPING_DETAIL_HPP

/* Chimera Includes */
#include <Chimera/src/common/preprocessor.hpp>

#if defined( USING_STL_THREADS )
#include <Chimera/src/threading/stl/stl_sync_primitives.hpp>
#elif defined( USING_FREERTOS_THREADS )
#include <Chimera/src/threading/freertos/freertos_hooks.hpp>
#include <Chimera/src/threading/freertos/freertos_sync_primitives.hpp>
#endif 

#endif	/* !CHIMERA_THREADING_OS_TYPING_DETAIL_HPP */
