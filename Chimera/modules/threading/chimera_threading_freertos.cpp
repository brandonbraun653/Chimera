/********************************************************************************
 *  File Name:
 *    chimera_threading_freertos.cpp
 *
 *  Description:
 *    Specializes the threading primitives interface for FreeRTOS
*
*  2019 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/threading.hpp>


#if defined( USING_FREERTOS ) && ( CHIMERA_CFG_FREERTOS == 1 )

/* FreeRTOS Includes */
#ifdef __cplusplus
extern "C"
{
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
}
#endif /* __cplusplus */

namespace Chimera::Threading
{
  void yield()
  {
    taskYIELD();
  }
}

#endif /* CHIMERA_CFG_FREERTOS */
