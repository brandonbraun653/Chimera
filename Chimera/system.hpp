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
#include <Chimera/interface/system_intf.hpp>
#include "chimeraPort.hpp"

namespace Chimera::System
{
  /**
   *  Performs core system initialization functionality for both Chimera
   *  and the attached backend driver. Once this function exits, it is 
   *  assumed that the MCU is completely initialized and ready to execute
   *  user code.
   *
   *  @warning Must be the first call upon entry into user code (typically main())
   *
   *  @return Chimera::Status_t
   */
  Chimera::Status_t initialize();

#if !defined( CHIMERA_INHERITED_SYSCTL )
  using CHIMERA_INHERITED_SYSCTL = ControlUnsupported;
#endif

  class SystemControl : public CHIMERA_INHERITED_SYSCTL
  {
  public:
    SystemControl()  = default;
    ~SystemControl() = default;

  private:
  };

  static_assert( std::is_base_of<ControlInterface, SystemControl>::value, "Class implements incorrect interface" );


#if !defined( CHIMERA_INHERITED_SYSTEM_IDENTIFIER )
  using CHIMERA_INHERITED_SYSTEM_IDENTIFIER = IdentifierUnsupported;
#endif

  class Identifier : public CHIMERA_INHERITED_SYSTEM_IDENTIFIER
  {
  public:
    Identifier()  = default;
    ~Identifier() = default;
  };
  static_assert( std::is_base_of<IdentifierInterface, Identifier>::value, "Class implements incorrect interface" );
}  // namespace Chimera::System

#endif /* !CHIMERA_SYSTEM_HPP */