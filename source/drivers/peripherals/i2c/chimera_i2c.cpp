/******************************************************************************
 *  File Name:
 *    chimera_i2c.cpp
 *
 *  Description:
 *    Chimera I2C driver hook implementation
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/i2c>

namespace Chimera::I2C
{
  /*---------------------------------------------------------------------------
  Static Data
  ---------------------------------------------------------------------------*/
  static Backend::DriverConfig s_backend_driver;

  /*---------------------------------------------------------------------------
  Public Functions
  ---------------------------------------------------------------------------*/
  namespace Backend
  {
    Chimera::Status_t __attribute__( ( weak ) ) registerDriver( Chimera::I2C::Backend::DriverConfig &registry )
    {
      registry.isSupported = false;
      return Chimera::Status::NOT_SUPPORTED;
    }
  }


  Chimera::Status_t initialize()
  {
    memset( &s_backend_driver, 0, sizeof( s_backend_driver ) );

    /*------------------------------------------------
    Register the backend interface with Chimera
    ------------------------------------------------*/
    auto result = Backend::registerDriver( s_backend_driver );
    if ( result != Chimera::Status::OK )
    {
      return result;
    }

    /*------------------------------------------------
    Try and invoke the registered init sequence
    ------------------------------------------------*/
    if ( s_backend_driver.isSupported && s_backend_driver.initialize )
    {
      return s_backend_driver.initialize();
    }
    else
    {
      return Chimera::Status::NOT_SUPPORTED;
    }

    return result;
  }


  Chimera::Status_t reset()
  {
    if ( s_backend_driver.isSupported && s_backend_driver.reset )
    {
      return s_backend_driver.reset();
    }
    else
    {
      return Chimera::Status::NOT_SUPPORTED;
    }
  }


  Driver_rPtr getDriver( const Channel channel )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.getDriver )
    {
      return s_backend_driver.getDriver( channel );
    }
    else
    {
      return nullptr;
    }
  }

}  // namespace Chimera::I2C
