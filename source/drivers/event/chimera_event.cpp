/********************************************************************************
 *  File Name:
 *    chimera_event.cpp
 *
 *  Description:
 *    Implements the event functionality for Chimera
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* STL Includes */


/* Chimera Includes */
#include <Chimera/event>
#include <Chimera/thread>

namespace Chimera::Event
{
  static bool processListener_Atomic( const Trigger event, Actionable &listener, uint32_t value );
  static bool processListener_UserCallback( const Trigger event, Actionable &listener, uint32_t value );
  static bool processListener_ISRCallback( const Trigger event, Actionable &listener, uint32_t value );
  static bool processListener_Semaphore( const Trigger event, Actionable &listener, uint32_t value );


  bool notifyListener( const Trigger event, Actionable &listener, uint32_t value )
  {
    switch ( listener.type )
    {
      case ListenerType::LISTENER_ATOMIC:
        return processListener_Atomic( event, listener, value );
        break;

      case ListenerType::LISTENER_SEMAPHORE:
        return processListener_Semaphore( event, listener, value );
        break;

      case ListenerType::LISTENER_FUNCTION:
        return processListener_UserCallback( event, listener, value );
        break;

      case ListenerType::LISTENER_ISR_CALLBACK:
        return processListener_ISRCallback( event, listener, value );
        break;

      case ListenerType::LISTENER_INVALID:
      default:
        return false;
        break;
    }
  }

  bool notifyListenerList( const Trigger event, ActionableList &list, uint32_t value )
  {
    bool accumulatedResult = true;

    for ( Actionable &item : list )
    {
      accumulatedResult |= notifyListener( event, item, value );
    }

    return accumulatedResult;
  }

  bool processListener_Atomic( const Trigger event, Actionable &listener, uint32_t value )
  {
    /*------------------------------------------------
    Only listen if we actually care about the event
    ------------------------------------------------*/
    if ( listener.trigger != event )
    {
      return false;
    }

    /*------------------------------------------------
    Make sure we don't perform a nullptr dereference
    ------------------------------------------------*/
    if ( listener.object.atomic_listener )
    {
      *( listener.object.atomic_listener ) = value;
      return true;
    }
    else
    {
      return false;
    }
  }

  bool processListener_Semaphore( const Trigger event, Actionable &listener, uint32_t value )
  {
    /*------------------------------------------------
    Only listen if we actually care about the event
    ------------------------------------------------*/
    if ( listener.trigger != event )
    {
      return false;
    }

    /*------------------------------------------------
    Make sure we don't perform a nullptr dereference
    ------------------------------------------------*/
    if ( listener.object.semaphore_listener )
    {
      listener.object.semaphore_listener->releaseFromISR();
      return true;
    }
    else
    {
      return false;
    }
  }

  bool processListener_ISRCallback( const Trigger event, Actionable &listener, uint32_t value )
  {
    /*------------------------------------------------
    Only listen if we actually care about the event
    ------------------------------------------------*/
    if ( listener.trigger != event )
    {
      return false;
    }

    /*------------------------------------------------
    Make sure we don't perform a nullptr dereference
    ------------------------------------------------*/
    if ( listener.object.isr_callback_listener )
    {
      listener.object.isr_callback_listener( &value, sizeof( value ) );
      return true;
    }
    else
    {
      return false;
    }
  }

  bool processListener_UserCallback( const Trigger event, Actionable &listener, uint32_t value )
  {
    /*------------------------------------------------
    Only listen if we actually care about the event
    ------------------------------------------------*/
    if ( listener.trigger != event )
    {
      return false;
    }

    /*------------------------------------------------
    Make sure we don't perform a nullptr dereference
    ------------------------------------------------*/
    if ( listener.object.event_callback_listener )
    {
      listener.object.event_callback_listener( static_cast<size_t>( event ), &value, sizeof( value ) );
      return true;
    }
    else
    {
      return false;
    }
  }
}  // namespace Chimera::Event
