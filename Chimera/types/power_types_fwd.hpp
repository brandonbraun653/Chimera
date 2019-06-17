/********************************************************************************
 *   File Name:
 *       power_types_fwd.hpp
 *
 *   Description:
 *       Forward declares types that will ultimately be defined by the backend
 *       driver that hooks into Chimera. This allows function declarations without
 *       knowing the full details of the types.
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_POWER_TYPES_FWD_HPP
#define CHIMERA_POWER_TYPES_FWD_HPP

namespace Chimera::Power
{
  /**
   *  Enumerates the available power states for the processor.
   */
  enum class State;
}  // namespace Chimera::Power

#endif /* !CHIMERA_POWER_TYPES_FWD_HPP */