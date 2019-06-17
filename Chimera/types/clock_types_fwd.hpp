/********************************************************************************
 *   File Name:
 *       clock_types_fwd.hpp
 *
 *   Description:
 *       Forward declares types that will ultimately be defined by the backend
 *       driver that hooks into Chimera. This allows function declarations without
 *       knowing the full details of the types.
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_CLOCK_TYPES_FWD_HPP
#define CHIMERA_CLOCK_TYPES_FWD_HPP

namespace Chimera::Clock
{
  /**
   *  The type of clock busses supported by the MCU. For example on the
   *  STM32, the declaration of this type could be:
   *
   *  @code{.cpp}
   *    enum class Bus : size_t
   *    {
   *      SYSCLK,
   *      HSI,
   *      HSE,
   *      ...
   *    };
   *
   *  @endcode
   */
  enum class Bus;
}  // namespace Chimera::Clock

#endif /* !CHIMERA_CLOCK_TYPES_FWD_HPP */