/********************************************************************************
*  File Name:
*    internal_types.hpp
*
*  Description:
*    Includes OS specific types for threading primitives    
*
*  2020 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/

#ifndef CHIMERA_THREADING_OS_TYPING_DETAIL_HPP
#define CHIMERA_THREADING_OS_TYPING_DETAIL_HPP

/* Chimera Includes */
#include <Chimera/chimera.hpp>

#if defined( USING_WINDOWS ) || defined( USING_LINUX ) || defined( USING_STL_THREADS )
#include <Chimera/threading/stl/stl_sync_primitives.hpp>
#elif defined( USING_FREERTOS ) || defined( USING_FREERTOS_THREADS )
#include <Chimera/threading/freertos/freertos_sync_primitives.hpp>
#endif 

#endif	/* !CHIMERA_THREADING_OS_TYPING_DETAIL_HPP */
