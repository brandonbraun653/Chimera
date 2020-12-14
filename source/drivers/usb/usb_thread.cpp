/********************************************************************************
 *  File Name:
 *    chimera_usb_thread.cpp
 *
 *  Description:
 *    USB thread implementation
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Chimera Includes */
#include <Chimera/thread>
#include <Chimera/usb>

namespace Chimera::USB
{
  /*-------------------------------------------------------------------------------
  Static Data
  -------------------------------------------------------------------------------*/
  static bool s_usb_online    = false;
  static size_t s_update_rate = 25;
  static Chimera::Threading::RecursiveMutex s_driver_lock;


  /*-------------------------------------------------------------------------------
  Static Functions
  -------------------------------------------------------------------------------*/
  /**
   *  Locks the USB mutex and halts execution of the USB thread
   *  @return void
   */
  static void lock_process();

  /**
   *  Unlocks the USB mutex and resumes execution of the USB thread
   *  @return void
   */
  static void unlock_process();

  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  bool isServiceRunning()
  {
    return s_usb_online;
  }


  bool resetServices()
  {
    // TODO: Implement me. Might need a thread restart mechanism.
    return false;
  }


  void setProcessingRate( const size_t rate )
  {
    lock_process();
    s_update_rate = rate;
    unlock_process();
  }


  void USBMainThread( void *arg )
  {
    /*-------------------------------------------------------------------------------
    System Initialization
    -------------------------------------------------------------------------------*/


    /*-------------------------------------------------------------------------------
    Main Loop
    -------------------------------------------------------------------------------*/
    while ( 1 )
    {
      Chimera::Threading::this_thread::yield();
    }
  }

  /*-------------------------------------------------------------------------------
  Static Function Implementations
  -------------------------------------------------------------------------------*/
  static void lock_process()
  {
    s_driver_lock.lock();
    Chimera::Threading::getThread( USBThreadName.cbegin() )->suspend();
  }

  static void unlock_process()
  {
    s_driver_lock.unlock();
    Chimera::Threading::getThread( USBThreadName.cbegin() )->resume();
  }
}  // namespace Chimera::USB
