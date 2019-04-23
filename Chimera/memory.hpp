/********************************************************************************
 * File Name:
 *	  memory.hpp
 *
 * Description:
 *	  Interface to various kinds of memory that Chimera supports
 *
 * 2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_MEMORY_HPP
#define CHIMERA_MEMORY_HPP

/* C++ Includes */
#include <cstdint>
#include <memory>

/* Chimera Includes */
#include "chimeraPort.hpp"
#include <Chimera/interface/macro.hpp>
#include <Chimera/interface/memory_intf.hpp>

namespace Chimera
{
  namespace Memory
  {
#if !defined( CHIMERA_INHERITED_SYSTEM_FLASH )
    using CHIMERA_INHERITED_SYSTEM_FLASH = SystemFlashUnsupported;
#endif
    
    class InternalFlash : public CHIMERA_INHERITED_SYSTEM_FLASH
    {
    public:
      InternalFlash()  = default;
      ~InternalFlash() = default;
    };

    static_assert( std::is_base_of<Chimera::Modules::Memory::GenericInterface, InternalFlash>::value,
                   "Class implements wrong interface" );

#if !defined( CHIMERA_DISABLE_INHERITANCE_WARNINGS )
    STATIC_WARNING( !( std::is_base_of<SystemFlashUnsupported, InternalFlash>::value ),
                    "No system Flash interface defined in backend driver. You can disable these warnings by defining "
                    "CHIMERA_DISABLE_INHERITANCE_WARNINGS in the preprocessor." );
#endif
    
    

#if !defined( CHIMERA_INHERITED_SYSTEM_SRAM )
    using CHIMERA_INHERITED_SYSTEM_SRAM = SystemSRAMUnsupported;
#endif
    
    class InternalSRAM : public CHIMERA_INHERITED_SYSTEM_SRAM
    {
    public:
      InternalSRAM()  = default;
      ~InternalSRAM() = default;
    };

    static_assert( std::is_base_of<Chimera::Modules::Memory::GenericInterface, InternalSRAM>::value,
                   "Class implements wrong interface" );

#if !defined( CHIMERA_DISABLE_INHERITANCE_WARNINGS )
    STATIC_WARNING( !( std::is_base_of<SystemSRAMUnsupported, InternalSRAM>::value ),
                    "No system SRAM interface defined in backend driver. You can disable these warnings by defining "
                    "CHIMERA_DISABLE_INHERITANCE_WARNINGS in the preprocessor." );
#endif
    
  }  // namespace Memory
}  // namespace Chimera

#endif /* !CHIMERA_MEMORY_HPP */