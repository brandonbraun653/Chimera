/********************************************************************************
 *  File Name:
 *    freertos_hooks_prj.hpp
 *
 *  Description:
 *    Allows the user to register various functions as callbacks into common
 *    FreeRTOS hooks.
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/
#pragma once
#ifndef CHIMERA_FREERTOS_HOOKS_PRJ_HPP
#define CHIMERA_FREERTOS_HOOKS_PRJ_HPP

/* Chimera Includes */
#include "chimeraConfig.hpp"
#include <Chimera/chimera.hpp>
#if defined( USING_FREERTOS ) && (CHIMERA_CFG_FREERTOS == 1 )

/* FreeRTOS Includes */
#include "FreeRTOS.h"
#include "task.h"

namespace Chimera::Modules::FreeRTOS
{
  extern void ApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName);

  extern void ApplicationTickHook();

  extern void ApplicationMallocFailedHook();

  extern void ApplicationIdleHook();

}  // namespace Chimera::Modules::FreeRTOS
#endif /* CHIMERA_CFG_FREERTOS */

#endif /* !CHIMERA_FREERTOS_HOOKS_PRJ_HPP */
