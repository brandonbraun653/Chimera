/********************************************************************************
 *  File Name:
 *    usb_user.hpp
 *
 *  Description:
 *    Implements an interface to create a Chimera USB peripheral
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_USB_USER_HPP
#define CHIMERA_USB_USER_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/source/drivers/peripherals/usb/usb_intf.hpp>
#include <Chimera/source/drivers/peripherals/usb/usb_types.hpp>

/**
 *  These functions shouldn't be used by the end consumer. See the high level
 *  USB driver interface (usb_intf.hpp) for a description of why.
 */
namespace Chimera::USB::Peripheral
{
  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  Chimera::Status_t initialize();
  Chimera::Status_t reset();
  Driver_rPtr getDriver( const Channel channel );
}  // namespace Chimera::USB::Peripheral

#endif /* !CHIMERA_USB_USER_HPP */
