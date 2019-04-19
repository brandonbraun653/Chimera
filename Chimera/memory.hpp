/********************************************************************************
 * File Name:
 *	  memory.hpp
 *
 * Description:
 *	  Interface to various kinds of memory that Chimera supports
 *
 * 2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */
#include <cstdint>
#include <memory>

/* Chimera Includes */
#include <Chimera/interface/macro.hpp>
#include <Chimera/interface/memory_intf.hpp>

namespace Chimera
{
  namespace Memory
  {
#if !defined( CHIMERA_INHERITED_SYSTEM_FLASH )
    using CHIMERA_INHERITED_SYSTEM_FLASH = SystemFlashUnsupported;
#endif
    
    class SystemFlash : public CHIMERA_INHERITED_SYSTEM_FLASH
    {
    public:
      SystemFlash()  = default;
      ~SystemFlash() = default;
    };

    static_assert( std::is_base_of<Chimera::Modules::Memory::GenericInterface, SystemFlash>::value,
                   "Class implements wrong interface" );

#if !defined( CHIMERA_DISABLE_INHERITANCE_WARNINGS )
    STATIC_WARNING( !( std::is_base_of<SystemFlashUnsupported, SystemFlash>::value ),
                    "No system Flash interface defined in backend driver. You can disable these warnings by defining "
                    "CHIMERA_DISABLE_INHERITANCE_WARNINGS in the preprocessor." );
#endif
    
    

#if !defined( CHIMERA_INHERITED_SYSTEM_SRAM )
    using CHIMERA_INHERITED_SYSTEM_SRAM = SystemSRAMUnsupported;
#endif
    
    class SystemSRAM : public CHIMERA_INHERITED_SYSTEM_SRAM
    {
    public:
      SystemSRAM()  = default;
      ~SystemSRAM() = default;
    };

    static_assert( std::is_base_of<Chimera::Modules::Memory::GenericInterface, SystemSRAM>::value,
                   "Class implements wrong interface" );

#if !defined( CHIMERA_DISABLE_INHERITANCE_WARNINGS )
    STATIC_WARNING( !( std::is_base_of<SystemSRAMUnsupported, SystemSRAM>::value ),
                    "No system SRAM interface defined in backend driver. You can disable these warnings by defining "
                    "CHIMERA_DISABLE_INHERITANCE_WARNINGS in the preprocessor." );
#endif
    
  }  // namespace Memory
}  // namespace Chimera