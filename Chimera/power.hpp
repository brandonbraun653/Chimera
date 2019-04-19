/********************************************************************************
 *   File Name:
 *    power.hpp
 *
 *   Description:
 *    Models the system power interface 
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once 
#ifndef CHIMERA_POWER_HPP
#define CHIMERA_POWER_HPP

/* Chimera Includes */
#include <Chimera/interface/macro.hpp>
#include <Chimera/interface/power_intf.hpp>
#include "chimeraPort.hpp"

namespace Chimera
{
  namespace Power
  {
#if !defined( CHIMERA_INHERITED_POWER_INFO )
    using CHIMERA_INHERITED_POWER_INFO = InfoInterfaceDisabled;
#endif

    /**
     *  A simple wrapper to provide a common class type for programs built
     *  with Chimera. The runtime behavior of this class is defined by the user
     *  provided class type CHIMERA_INHERITED_POWER in chimeraPort.hpp.
     *
     *  If no user class is provided, a default disabled behavior version will be
     *  substituted in its place.
     */
    class Info : public CHIMERA_INHERITED_POWER_INFO
    {
    public:
      Info() : CHIMERA_INHERITED_POWER_INFO(){};
      ~Info() = default;
    };
  
    static_assert( std::is_base_of<InfoInterface, Info>::value, "Base class implements the wrong interface" );

#if !defined( CHIMERA_DISABLE_INHERITANCE_WARNINGS )
    STATIC_WARNING( !( std::is_base_of<InfoInterfaceDisabled, Info>::value ),
                    "No power interface defined in backend driver. You can disable these warnings by defining "
                    "CHIMERA_DISABLE_INHERITANCE_WARNINGS in the preprocessor." );
#endif

  }  // namespace Power
}  // namespace Chimera


#endif /* !CHIMERA_POWER_HPP */