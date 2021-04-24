/********************************************************************************
 *  File Name:
 *    usb_intf.cpp
 *
 *  Description:
 *    Implements the high level USB driver interface
 *
 *  2020-2021 | Brandon Braun | brandonbraun653@gmail.com
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
  Chimera::Status_t init( const Channel ch, const PeriphConfig &cfg )
  {
    using namespace Chimera::Thread;

    /*-------------------------------------------------
    Input protection
    -------------------------------------------------*/
    if ( !( ch < Channel::NUM_OPTIONS ) )
    {
      return Chimera::Status::INVAL_FUNC_PARAM;
    }

    /*-------------------------------------------------
    Start the main thread. All the hardware driver core
    init sequences should've already been called.
    -------------------------------------------------*/
    auto result = Chimera::Status::OK;
    if ( getThread( USBThreadName ) == nullptr )
    {
      Task usbThread;
      TaskConfig cfg;

      cfg.arg        = nullptr;
      cfg.function   = USBMainThread;
      cfg.priority   = USBDefaultPriority;
      cfg.stackWords = USBDefaultStackSize;
      cfg.type       = TaskInitType::DYNAMIC;
      cfg.name       = USBThreadName.data();

      usbThread.create( cfg );
      usbThread.start();

      if ( result != Chimera::Status::OK )
      {
        return result;
      }
    }

    /*-------------------------------------------------
    Initialize the driver associated with the channel
    -------------------------------------------------*/
    if ( auto driver = Peripheral::getDriver( ch ); driver != nullptr )
    {
      result = driver->open( cfg );
    }
    else
    {
      result = Chimera::Status::NOT_FOUND;
    }

    return result;
  }


  Chimera::Status_t destroy( const Channel ch )
  {
    /*-------------------------------------------------
    Input protection
    -------------------------------------------------*/
    if ( !( ch < Channel::NUM_OPTIONS ) )
    {
      return Chimera::Status::INVAL_FUNC_PARAM;
    }

    /*-------------------------------------------------
    Initialize the driver associated with the channel
    -------------------------------------------------*/
    auto result = Chimera::Status::OK;
    if ( auto driver = Peripheral::getDriver( ch ); driver != nullptr )
    {
      driver->close();
    }
    else
    {
      result = Chimera::Status::NOT_FOUND;
    }

    return result;
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
