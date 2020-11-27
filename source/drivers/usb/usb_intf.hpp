/********************************************************************************
 *  File Name:
 *    usb_intf.hpp
 *
 *  Description:
 *    High level USB interface for application level drivers. Think libusb, but
 *    not as popular or as well thought out. It's a work in progress, ok?
 *
 *    This peripheral driver is a bit unique compared to other drivers in Chimera
 *    because the peripheral isn't actually supposed to be used directly by the
 *    consumer. USB is a bit more complex, so rather than let each user reinvent
 *    the wheel with some of the runtime control behavior, a dedicated thread
 *    handles this. The user's interface is reduced to the functions in this file
 *    which should hopefully simply integration into the project's application.
 *
 *  Note:
 *    This driver won't run properly without threading support.
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_USB_APPLICATION_INTERFACE_HPP
#define CHIMERA_USB_APPLICATION_INTERFACE_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/source/drivers/peripherals/usb/usb_types.hpp>


namespace Chimera::USB
{
  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  /**
   *  Initializes the USB system and tries to enumerate the device with the host
   *  controller. This is going to attempt to register the USBMain() thread with
   *  the system, so ensure that whatever RTOS in use has been started.
   *
   *  @param[in]  ch      Which USB peripheral to act on
   *  @return Chimera::Status_t
   */
  Chimera::Status_t init( const Channel ch );

  /**
   *  Resets the USB system as if a power cycle has occurred. Will require calling
   *  the init() method again to re-use the driver.
   *
   *  @param[in]  ch      Which USB peripheral to act on
   *  @return Chimera::Status_t
   */
  Chimera::Status_t reset( const Channel ch );

  /**
   *  Opens an endpoint for use
   *
   *  @param[in]  ch      Which USB peripheral to act on
   *  @param[in]  ep      The endpoint to configure
   *  @param[in]  cfg     Configuration info for the endpoint
   *  @return Chimera::Status_t
   */
  Chimera::Status_t open( const Channel ch, const Endpoint ep, const EndpointConfig &cfg );

  /**
   *  Closes an endpoint and releases all resources
   *
   *  @param[in]  ch      Which USB peripheral to act on
   *  @param[in]  ep      The endpoint to destroy
   *  @return Chimera::Status_t
   */
  Chimera::Status_t close( const Channel ch, const Endpoint ep );

  /**
   *  Reads a packet of data from the given endpoint
   *
   *  @param[in]  ch      Which USB peripheral to act on
   *  @param[in]  ep      The endpoint to read from
   *  @param[out] data    Memory buffer to write the data into
   *  @param[in]  size    Size of the memory buffer in bytes
   *  @return Chimera::Status_t
   */
  Chimera::Status_t read( const Channel ch, const Endpoint ep, void *const data, const size_t size );

  /**
   *  Writes a packet of data to the given endpoint
   *
   *  @param[in]  ch      Which USB peripheral to act on
   *  @param[in]  ep      The endpoint to write to
   *  @param[in]  data    Memory buffer to copy from
   *  @param[in]  size    Number of bytes to copy from the memory buffer
   *  @return Chimera::Status_t
   */
  Chimera::Status_t write( const Channel ch, const Endpoint ep, const void *const data, const size_t size );

}  // namespace Chimera::USB

#endif /* !CHIMERA_USB_APPLICATION_INTERFACE_HPP */
