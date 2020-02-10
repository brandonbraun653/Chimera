/********************************************************************************
 *  File Name:
 *    chimera_crc.cpp
 *
 *  Description:
 *    Implements the software CRC algorithm
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Chimera Includes */
#include "chimeraPort.hpp"
#include <Chimera/crc>

namespace Chimera::HWCRC
{
#if !defined( CHIMERA_INHERITED_HWCRC )
  using CHIMERA_INHERITED_HWCRC = HWInterfaceUnsupported;
#endif

  static_assert( std::is_base_of<HWInterfaceUnsupported, CHIMERA_INHERITED_HWCRC>::value, "Invalid interface" );


  Chimera::Status_t initialize()
  {
    return Chimera::CommonStatusCodes::OK;
  }

  HWCRC_sPtr create_shared_ptr()
  {
    return std::make_shared<CHIMERA_INHERITED_HWCRC>();
  }

  HWCRC_uPtr create_unique_ptr()
  {
    return std::make_unique<CHIMERA_INHERITED_HWCRC>();
  }
}  // namespace Chimera::HWCRC
