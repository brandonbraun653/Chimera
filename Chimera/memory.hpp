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
#include <Chimera/interface.hpp>

namespace Chimera
{
  namespace Memory
  {
    class SystemFlash : public CHIMERA_INHERITED_SYSTEM_FLASH
    {
    public:
      SystemFlash() = default;
      ~SystemFlash() = default;
    };

    using SystemFlash_sPtr = std::shared_ptr<SystemFlash>;
    using SystemFlash_uPtr = std::unique_ptr<SystemFlash>;
    
    static_assert( std::is_base_of<Chimera::Modules::Memory::GenericInterface, SystemFlash>::value, "Class implements wrong interface" );

    
    class SystemSRAM : public CHIMERA_INHERITED_SYSTEM_SRAM
    {
    public:
      SystemSRAM() = default;
      ~SystemSRAM() = default;
    };

    using SystemSRAM_sPtr = std::shared_ptr<SystemSRAM>;
    using SystemSRAM_uPtr = std::unique_ptr<SystemSRAM>;
    
    static_assert( std::is_base_of<Chimera::Modules::Memory::GenericInterface, SystemSRAM>::value, "Class implements wrong interface" );

  }  // namespace Memory
}  // namespace Chimera