/******************************************************************************
 *  File Name:
 *    chimera_sdio.cpp
 *
 *  Description:
 *    Chimera SDIO driver implementation
 *
 *  2023 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/cfg>
#include <Chimera/sdio>


namespace Chimera::SDIO
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
#if CHIMERA_DEFAULT_DRIVER_REGISTRATION == 1
    Chimera::Status_t __attribute__( ( weak ) ) registerDriver( DriverConfig &registry )
    {
      registry.isSupported = false;
      return Chimera::Status::NOT_SUPPORTED;
    }
#endif /* CHIMERA_DEFAULT_DRIVER_REGISTRATION */
  }    // namespace Backend


  Chimera::Status_t initialize()
  {
    /*-------------------------------------------------------------------------
    Register the backend interface with Chimera
    -------------------------------------------------------------------------*/
    s_backend_driver.clear();
    auto result = Backend::registerDriver( s_backend_driver );

    if ( result != Chimera::Status::OK )
    {
      return result;
    }

    /*-------------------------------------------------------------------------
    Try and invoke the registered init sequence
    -------------------------------------------------------------------------*/
    if ( s_backend_driver.isSupported && s_backend_driver.initialize )
    {
      return s_backend_driver.initialize();
    }
    else
    {
      return Chimera::Status::NOT_SUPPORTED;
    }
  }


  Chimera::Status_t reset()
  {
    if ( s_backend_driver.isSupported && s_backend_driver.reset )
    {
      auto reset_func_ptr = s_backend_driver.reset;
      s_backend_driver.clear();

      return reset_func_ptr();
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
}  // namespace Chimera::SDIO
