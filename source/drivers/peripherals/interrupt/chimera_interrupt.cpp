/******************************************************************************
 *  File Name:
 *    chimera_interrupt.cpp
 *
 *  Description:
 *    Backend registration for the interrupt layer
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/interrupt>

namespace Chimera::Interrupt
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
    Chimera::Status_t __attribute__( ( weak ) ) registerDriver( Chimera::Interrupt::Backend::DriverConfig &registry )
    {
      registry.isSupported = false;
      return Chimera::Status::NOT_SUPPORTED;
    }
  }  // namespace Backend

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


  Chimera::Status_t registerISRHandler( const Peripheral::Type type, const Signal_t signal, const SignalCallback &callback )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.registerISRHandler )
    {
      return s_backend_driver.registerISRHandler( type, signal, callback );
    }
    else
    {
      return Chimera::Status::NOT_SUPPORTED;
    }
  }


  Chimera::Status_t disableISR( const Signal_t signal )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.disableISR )
    {
      return s_backend_driver.disableISR( signal );
    }
    else
    {
      return Chimera::Status::NOT_SUPPORTED;
    }
  }


  Chimera::Status_t enableISR( const Signal_t signal )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.enableISR )
    {
      return s_backend_driver.enableISR( signal );
    }
    else
    {
      return Chimera::Status::NOT_SUPPORTED;
    }
  }

}  // namespace Chimera::Interrupt
