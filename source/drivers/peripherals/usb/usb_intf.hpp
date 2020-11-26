/********************************************************************************
 *  File Name:
 *    usb_intf.hpp
 *
 *  Description:
 *    Interface to the USB peripheral
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_USB_INTERFACE_HPP
#define CHIMERA_USB_INTERFACE_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/event>
#include <Chimera/source/drivers/peripherals/usb/usb_types.hpp>
#include <Chimera/thread>

namespace Chimera::USB
{
  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  namespace Backend
  {
    /**
     *  Registers the backend driver with Chimera
     *
     *  @param[in]  registry    Chimera's copy of the driver interface
     *  @return Chimera::Status_t
     */
    extern Chimera::Status_t registerDriver( DriverConfig &registry );
  }  // namespace Backend


  /*-------------------------------------------------------------------------------
  Classes
  -------------------------------------------------------------------------------*/
  /**
   *  Defines expected interface and behavior for a USB driver
   */
  class HWInterface
  {
  public:
    ~HWInterface() = default;
  };

  /**
   *  Virtual class to facilitate easy mocking of the driver
   */
  class IUSB : virtual public HWInterface, virtual public Chimera::Threading::LockableInterface
  {
  public:
    virtual ~IUSB() = default;
  };


  /**
   *  Concrete class declaration that promises to implement the virtual one, to
   *  avoid paying the memory penalty of a v-table lookup. Implemented project side.
   */
  class Driver
  {
  public:
    Driver();
    ~Driver();

    /*-------------------------------------------------
    Interface: Hardware
    -------------------------------------------------*/


    /*-------------------------------------------------
    Interface: Lockable
    -------------------------------------------------*/
    void lock();
    void lockFromISR();
    bool try_lock_for( const size_t timeout );
    void unlock();
    void unlockFromISR();

  private:
    void *mDriver; /**< Opaque pointer to the underlying implementation */
  };

}  // namespace Chimera::USB

#endif /* !CHIMERA_USB_INTERFACE_HPP */
