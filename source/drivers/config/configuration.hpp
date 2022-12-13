/******************************************************************************
 *  File Name:
 *    configuration.hpp
 *
 *  Description:
 *    Configuration information controlling Chimera behavior
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_CONFIGURATION_HPP
#define CHIMERA_CONFIGURATION_HPP

namespace Chimera::Config
{
/* clang-format off */
  // Convenience flag for determining if a configuration feature is disabled
  #ifndef CHIMERA_ENABLE
  #define CHIMERA_ENABLE    ( 1 )
  #endif

  // Convenience flag for determining if a configuration feature is disabled
  #ifndef CHIMERA_DISABLE
  #define CHIMERA_DISABLE   ( 0 )
  #endif

  // Default enable the infinite lifetime expectation in the driver interface
  #ifndef CHIMERA_DRIVER_INF_LIFETIME
  #define CHIMERA_DRIVER_INF_LIFETIME ( CHIMERA_ENABLE )
  #endif

  /**
   *  There are several different models for how a particular peripheral
   *  driver could be created. On the one hand, a new instance is made each
   *  time and all instances work together to not conflict with each other.
   *  On the other, the driver model follows the Singleton pattern. Once it
   *  is created, that is the only instance to exist and other objects simply
   *  get a reference to it.
   *
   *  The driver model of Chimera uses factory functions to create pointers to
   *  instances of the peripheral drivers. This could be shared_ptrs, unique_ptrs,
   *  or raw pointers. In the case of raw pointers, there exists the possibility
   *  that the back end driver will create and hand out the address to the driver
   *  instance and then in the future delete the instance, which is unsafe.
   *
   *  This flag allows for C style macro and C++ style constexpr operations to
   *  configure sections of code based on the expected driver lifetime. By default
   *  the raw pointer interface to device drivers will be disabled unless these
   *  flags have been set to true. This is a protective mechanism.
   */
  #if defined( CHIMERA_DRIVER_INF_LIFETIME ) && ( CHIMERA_DRIVER_INF_LIFETIME == CHIMERA_ENABLE )
  static constexpr bool DriverInfiniteLifetime = true;
  #else
  static constexpr bool DriverInfiniteLifetime = false;
  #endif

/* clang-format on */
}  // namespace Chimera::Config

#endif /* !CHIMERA_CONFIGURATION_HPP */
