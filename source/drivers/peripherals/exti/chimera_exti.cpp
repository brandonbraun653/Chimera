/********************************************************************************
 *  File Name:
 *    chimera_exti.cpp
 *
 *  Description:
 *    Implementation of Chimera side details for the External Interrupt driver
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* STL Includes */
#include <cstring>

/* Chimera Includes */
#include <Chimera/exti>

namespace Chimera::EXTI
{
  static Backend::DriverConfig s_backend_driver;

  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  Chimera::Status_t open()
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
    if ( s_backend_driver.isSupported && s_backend_driver.open )
    {
      return s_backend_driver.open();
    }
    else
    {
      return Chimera::Status::NOT_SUPPORTED;
    }

    return result;
  }


  Chimera::Status_t close()
  {
    if ( s_backend_driver.isSupported && s_backend_driver.close )
    {
      return s_backend_driver.close();
    }
    else
    {
      return Chimera::Status::NOT_SUPPORTED;
    }
  }


  Chimera::Status_t attach( const EventLine_t listener, Chimera::Function::vGeneric callback )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.attach )
    {
      return s_backend_driver.attach( listener, callback );
    }
    else
    {
      return Chimera::Status::NOT_SUPPORTED;
    }
  }


  Chimera::Status_t detach( const EventLine_t listener )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.detach )
    {
      return s_backend_driver.detach( listener );
    }
    else
    {
      return Chimera::Status::NOT_SUPPORTED;
    }
  }


  Chimera::Status_t trigger( const EventLine_t listener )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.trigger )
    {
      return s_backend_driver.trigger( listener );
    }
    else
    {
      return Chimera::Status::NOT_SUPPORTED;
    }
  }


  Chimera::Status_t disable( const EventLine_t listener )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.disable )
    {
      return s_backend_driver.disable( listener );
    }
    else
    {
      return Chimera::Status::NOT_SUPPORTED;
    }
  }


  Chimera::Status_t enable( const EventLine_t listener )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.enable )
    {
      return s_backend_driver.enable( listener );
    }
    else
    {
      return Chimera::Status::NOT_SUPPORTED;
    }
  }


  EventLine_t numInterruptLines()
  {
    if ( s_backend_driver.isSupported && s_backend_driver.numInterruptLines )
    {
      return s_backend_driver.numInterruptLines();
    }
    else
    {
      return Chimera::Status::NOT_SUPPORTED;
    }
  }

}  // namespace Chimera::EXTI
