/******************************************************************************
 *  File Name:
 *    configuration.hpp
 *
 *  Description:
 *    Configuration information controlling Chimera behavior
 *
 *  2020-2023 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_CONFIGURATION_HPP
#define CHIMERA_CONFIGURATION_HPP

#if __has_include("chimera_cfg_prj.hpp")
#include "chimera_cfg_prj.hpp"
#endif /* __has_include("chimera_cfg_prj.hpp") */

/*-----------------------------------------------------------------------------
Configuration Literals
-----------------------------------------------------------------------------*/
/*-------------------------------------------------------------------
Compile in a weakly defined default implementation of the HW driver
Chimera::<Driver>::Backend::registerDriver function. This is useful
for compiling purposes when a driver is not yet implemented and may
be overriden later by the Chimera interface implementer.
-------------------------------------------------------------------*/
#ifndef CHIMERA_DEFAULT_DRIVER_REGISTRATION
#define CHIMERA_DEFAULT_DRIVER_REGISTRATION  1
#endif

#endif /* !CHIMERA_CONFIGURATION_HPP */
