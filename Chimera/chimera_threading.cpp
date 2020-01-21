/********************************************************************************
 * File Name:
 *   threading.cpp
 *
 * Description:
 *   Implements the threading functions for Chimera
 *
 * 2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */
#include <cstring>
#include <vector>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/threading.hpp>
#include <Chimera/threading/types.hpp>
#include <Chimera/types/common_types.hpp>
#include <Chimera/watchdog.hpp>

#if defined( WIN32 ) || defined( WIN64 ) || defined( USING_FREERTOS )

namespace Chimera::Threading
{
}

#endif 