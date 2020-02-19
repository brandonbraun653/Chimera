/********************************************************************************
 *  File Name:
 *    chimera_crc.cpp
 *
 *  Description:
 *    Implements the software CRC algorithm
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* STL Includes */
#include <memory>
#include <cstring>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/crc>

namespace Chimera::HWCRC
{
  static Backend::DriverConfig s_backend_driver;

  Chimera::Status_t initialize()
  {
    memset( &s_backend_driver, 0, sizeof( s_backend_driver ) );
    return Backend::registerDriver( s_backend_driver );
  }

  Chimera::Status_t reset()
  {
    if ( s_backend_driver.isSupported && s_backend_driver.reset )
    {
      return s_backend_driver.reset();
    }
    else
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }
  }

  HWCRC_sPtr create_shared_ptr()
  {
    if ( s_backend_driver.isSupported && s_backend_driver.createShared )
    {
      return s_backend_driver.createShared();
    }
    else
    {
      return nullptr;
    }
  }

  HWCRC_uPtr create_unique_ptr()
  {
    if ( s_backend_driver.isSupported && s_backend_driver.createUnique )
    {
      return s_backend_driver.createUnique();
    }
    else
    {
      return nullptr;
    }
  }
}  // namespace Chimera::HWCRC
