/********************************************************************************
 *  File Name:
 *    usb_intf.cpp
 *
 *  Description:
 *    Implements the high level USB driver interface
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/thread>
#include <Chimera/usb>

namespace Chimera::USB
{
  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  Chimera::Status_t init()
  {
    return Chimera::Status::NOT_SUPPORTED;
  }


  Chimera::Status_t reset()
  {
    return Chimera::Status::NOT_SUPPORTED;
  }


  Chimera::Status_t open( const Channel ch, const Endpoint ep, const EndpointConfig &cfg )
  {
    return Chimera::Status::NOT_SUPPORTED;
  }


  Chimera::Status_t close( const Channel ch, const Endpoint ep )
  {
    return Chimera::Status::NOT_SUPPORTED;
  }


  Chimera::Status_t read( const Channel ch, const Endpoint ep, void *const data, const size_t size )
  {
    return Chimera::Status::NOT_SUPPORTED;
  }


  Chimera::Status_t write( const Channel ch, const Endpoint ep, const void *const data, const size_t size )
  {
    return Chimera::Status::NOT_SUPPORTED;
  }

}  // namespace Chimera::USB
