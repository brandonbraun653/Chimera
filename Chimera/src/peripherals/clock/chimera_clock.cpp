/********************************************************************************
 *  File Name:
 *    chimera_clock.cpp
 *
 *  Description:
 *    Implementation of the Chimera clock driver
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Chimera Includes */
#include <Chimera/clock>

namespace Chimera::Clock
{
  static Backend::DriverConfig s_backend_driver;

  Chimera::Status_t initialize()
  {
    /*------------------------------------------------
    Register the backend interface with Chimera
    ------------------------------------------------*/
    auto result = Backend::registerDriver( s_backend_driver );
    if ( result != Chimera::CommonStatusCodes::OK )
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

    return result;
  }

  Chimera::Status_t periphEnable( const Chimera::Peripheral::Type periph )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.periphEnable )
    {
      return s_backend_driver.periphEnable( periph );
    }
    else
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
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
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }
  }

  Chimera::Status_t enableClock( const Chimera::Clock::Bus bus )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.enableClock )
    {
      return s_backend_driver.enableClock( bus );
    }
    else
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }
  }

  Chimera::Status_t disableClock( const Chimera::Clock::Bus bus )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.disableClock )
    {
      return s_backend_driver.disableClock( bus );
    }
    else
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }
  }

  bool isEnabled( const Chimera::Clock::Bus bus )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.isEnabled )
    {
      return s_backend_driver.isEnabled( bus );
    }
    else
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }
  }

  size_t getFrequency( const Chimera::Clock::Bus bus )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.getFrequency )
    {
      return s_backend_driver.getFrequency( bus );
    }
    else
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }
  }

  Chimera::Status_t setFrequency( const Chimera::Clock::Bus bus, const size_t freq )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.setFrequency )
    {
      return s_backend_driver.setFrequency( bus, freq );
    }
    else
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }
  }

}  // namespace Chimera::Clock
