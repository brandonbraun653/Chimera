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
#include <Chimera/interface/macro.hpp>
#include <Chimera/interface/system_intf.hpp>
#include "chimeraPort.hpp"


namespace Chimera
{
  namespace System
  {
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

#if !defined( CHIMERA_DISABLE_INHERITANCE_WARNINGS )
    STATIC_WARNING( !( std::is_base_of<ControlUnsupported, SystemControl>::value ),
                    "No system control interface defined in backend driver. You can disable these warnings by defining "
                    "CHIMERA_DISABLE_INHERITANCE_WARNINGS in the preprocessor." );
#endif
    

    
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
#if !defined( CHIMERA_DISABLE_INHERITANCE_WARNINGS )
    STATIC_WARNING( !( std::is_base_of<IdentifierUnsupported, SystemControl>::value ),
                    "No system identifier interface defined in backend driver. You can disable these warnings by defining "
                    "CHIMERA_DISABLE_INHERITANCE_WARNINGS in the preprocessor." );
#endif
  }  // namespace System
}  // namespace Chimera

#endif /* !CHIMERA_SYSTEM_HPP */