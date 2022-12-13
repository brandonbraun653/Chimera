/******************************************************************************
 *  File Name:
 *    chimera_power.cpp
 *
 *	 Description:
 *    Implements the Chimera Power interface
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

/* STL Includes */
#include <memory>
#include <cstring>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/power>

namespace Chimera::Power
{
  static Backend::DriverConfig s_backend_driver;


  Chimera::Status_t initialize()
  {
    memset( &s_backend_driver, 0, sizeof( s_backend_driver ) );

    /*-------------------------------------------------------------------------
    Register the backend interface with Chimera
    -------------------------------------------------------------------------*/
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


  Chimera::Status_t periphEnable( const Chimera::Peripheral::Type periph )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.periphEnable )
    {
      return s_backend_driver.periphEnable( periph );
    }
    else
    {
      return Chimera::Status::NOT_SUPPORTED;
    }
  }


  Chimera::Status_t periphDisable( const Chimera::Peripheral::Type periph )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.periphDisable )
    {
      return s_backend_driver.periphDisable( periph );
    }
    else
    {
      return Chimera::Status::NOT_SUPPORTED;
    }
  }


  Chimera::Status_t setPowerState( const Chimera::Power::State state )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.setPowerState )
    {
      return s_backend_driver.setPowerState( state );
    }
    else
    {
      return Chimera::Status::NOT_SUPPORTED;
    }
  }


  Chimera::Power::State getPowerState( const Chimera::Peripheral::Type periph )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.getPowerState )
    {
      return s_backend_driver.getPowerState( periph );
    }
    else
    {
      return Chimera::Power::State::INVALID;
    }
  }

}  // namespace Chimera::Power
