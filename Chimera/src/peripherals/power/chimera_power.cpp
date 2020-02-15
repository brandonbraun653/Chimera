/********************************************************************************
 *  File Name:
 *    chimera_power.cpp
 *
 *	 Description:
 *    Implements the Chimera Power interface
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* STL Includes */
#include <memory>

/* Chimera Includes */
#include "chimeraPort.hpp"
#include <Chimera/common>
#include <Chimera/power>

namespace Chimera::Power
{
#if !defined( CHIMERA_INHERITED_POWER_INFO )
  using CHIMERA_INHERITED_POWER_INFO = InfoInterfaceDisabled;
  #warning Power driver is unsupported
#endif
  static_assert( std::is_base_of<InfoInterface, CHIMERA_INHERITED_POWER_INFO>::value, "Invalid interface" );


  Chimera::Status_t initialize()
  {
    return Chimera::CommonStatusCodes::OK;
  }

  Info_sPtr create_shared_info_ptr()
  {
    return std::make_shared<CHIMERA_INHERITED_POWER_INFO>();
  }

  Info_uPtr create_unique_info_ptr()
  {
    return std::make_unique<CHIMERA_INHERITED_POWER_INFO>();
  }

}  // namespace Chimera::Power