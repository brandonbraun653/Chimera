/********************************************************************************
 *   File Name:
 *       system.hpp
 *
 *   Description:
 *       Provides common system level functions
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SYSTEM_HPP
#define CHIMERA_SYSTEM_HPP

/* C++ Includes */
#include <cstdint>
#include <memory>
#include <type_traits>

/* Chimera Includes */
#include "chimeraPort.hpp"


namespace Chimera
{
  namespace System
  {
    class SystemControl : CHIMERA_INHERITED_SYSCTL
    {
    public:
      SystemControl()  = default;
      ~SystemControl() = default;

    private:
    };

    typedef std::shared_ptr<SystemControl> SystemControl_sPtr;
    typedef std::unique_ptr<SystemControl> SystemControl_uPtr;

    static_assert( std::is_base_of<Interface, SystemControl>::value,
                   "CHIMERA: Base class does not implement the correct interface" );
  }  // namespace System
}  // namespace Chimera

#endif /* !CHIMERA_SYSTEM_HPP */