/******************************************************************************
 *  File Name:
 *    chimera_adc.cpp
 *
 *  Description:
 *    Implements the Chimera ADC driver interface
 *
 *  2020-2025 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <cstring>
#include <Chimera/cfg>
#include <Chimera/adc>

namespace Chimera::ADC
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
    Chimera::Status_t __attribute__( ( weak ) ) registerDriver( Chimera::ADC::Backend::DriverConfig &registry )
    {
      registry.isSupported = false;
      return Chimera::Status::NOT_SUPPORTED;
    }
#endif /* CHIMERA_DEFAULT_DRIVER_REGISTRATION */
  }  // namespace Backend


  Chimera::Status_t initialize()
  {
    memset( &s_backend_driver, 0, sizeof( s_backend_driver ) );

    /*-------------------------------------------------------------------------
    Register the backend interface with Chimera
    -------------------------------------------------------------------------*/
    auto result = Backend::registerDriver( s_backend_driver );
    if( result != Chimera::Status::OK )
    {
      return result;
    }

    /*-------------------------------------------------------------------------
    Try and invoke the registered init sequence
    -------------------------------------------------------------------------*/
    if( s_backend_driver.isSupported && s_backend_driver.initialize )
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
    if( s_backend_driver.isSupported && s_backend_driver.reset )
    {
      return s_backend_driver.reset();
    }
    else
    {
      return Chimera::Status::NOT_SUPPORTED;
    }
  }


  Driver_rPtr getDriver( const Peripheral periph )
  {
    if( s_backend_driver.isSupported && s_backend_driver.getDriver )
    {
      return s_backend_driver.getDriver( periph );
    }
    else
    {
      return nullptr;
    }
  }


  bool featureSupported( const Peripheral periph, const Feature feature )
  {
    if( s_backend_driver.isSupported && s_backend_driver.getDriver )
    {
      return s_backend_driver.featureSupported( periph, feature );
    }
    else
    {
      return false;
    }
  }
}  // namespace Chimera::ADC
