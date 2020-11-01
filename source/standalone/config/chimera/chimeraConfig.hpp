/********************************************************************************
 *  File Name:
 *    chimeraConfig.hpp
 *
 *  Description:
 *    Default configuration definitions for including Chimera into a project
 *    without knowing what the backend will driver will be. This allows creation
 *    of static libraries that have no knowledge of who is actually implementing 
 *    Chimera.
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_STANDALONE_DEFAULT_CONFIGURATION_HPP
#define CHIMERA_STANDALONE_DEFAULT_CONFIGURATION_HPP

/* Enables support for FreeRTOS based multi-threading */
#ifndef CHIMERA_CFG_FREERTOS
#define CHIMERA_CFG_FREERTOS  0
#endif

/* */


#endif  // !CHIMERA_STANDALONE_DEFAULT_CONFIGURATION_HPP
