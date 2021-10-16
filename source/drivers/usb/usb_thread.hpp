/********************************************************************************
 *  File Name:
 *    chimera_usb_thread.hpp
 *
 *  Description:
 *    Declarations for the USB controller thread
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_USB_THREAD_HPP
#define CHIMERA_USB_THREAD_HPP

/* STL Includes */
#include <cstddef>
#include <string_view>

/* Chimera Includes */
#include <Chimera/thread>

namespace Chimera::USB
{
  /*-------------------------------------------------------------------------------
  Constants
  -------------------------------------------------------------------------------*/
  static const std::string_view USBThreadName                   = "USBMain";
  static const size_t USBDefaultStackSize                       = STACK_BYTES( 2048 );
  static const Chimera::Thread::TaskPriority USBDefaultPriority = 3;

  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  /**
   *  Query if the USB thread is running
   *  @return bool
   */
  bool isServiceRunning();

  /**
   *  Reset the USB thread should something go amiss
   *  @return void
   */
  bool resetServices();

  /**
   *  Configures the rate at which the main thread will periodically wake up
   *  and process USB data.
   *
   *  @param[in]  rate      Wakeup period in milliseconds
   *  @return void
   */
  void setProcessingRate( const size_t rate );

  /**
   *  Main thread of execution for running USB controller operations. Should
   *  be initialized by the system application with the Chimera::Thread library.
   *
   *  @warning Do not call directly. Initialized internally by the library.
   *
   *  @param[in]  arg     Unused, but required for thread function signature
   *  @return void
   */
  void USBMainThread( void *arg );


}  // namespace Chimera::USB

#endif /* !CHIMERA_USB_THREAD_HPP */
