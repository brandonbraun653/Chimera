/********************************************************************************
 *  File Name:
 *    extensions.hpp
 *
 *  Description:
 *    Threading extensions that implement some kind of helper objects
 *
 *  2020-2022 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_THREADING_EXTENSIONS_HPP
#define CHIMERA_THREADING_EXTENSIONS_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/source/drivers/common/chimera.hpp>
#include <Chimera/source/drivers/event/event_types.hpp>
#include <Chimera/source/drivers/threading/threading_abstract.hpp>
#include <Chimera/source/drivers/threading/threading_mutex.hpp>
#include <Chimera/source/drivers/threading/threading_semaphore.hpp>
#include <cstddef>
#include <cstdint>

namespace Chimera::Thread
{
  /**
   * @brief Analog to the std::lock_guard
   *
   * @tparam mutex_type
   */
  template<class mutex_type>
  class LockGuard
  {
  public:
    explicit LockGuard( mutex_type &mutex ) : mtx( &mutex )
    {
      mtx->lock();
    }

    ~LockGuard()
    {
      mtx->unlock();
    }

    LockGuard( const LockGuard & ) = delete;

  private:
    mutex_type *mtx;
  };

  /**
   * @brief Analog to the std::lock_guard, but with a timeout interface
   *
   * @tparam mutex_type
   */
  template<class mutex_type>
  class TimedLockGuard
  {
  public:
    explicit TimedLockGuard( mutex_type &mutex ) : mtx( mutex ), is_locked( false )
    {
    }

    ~TimedLockGuard()
    {
      if ( is_locked )
      {
        mtx.unlock();
      }
    }

    bool try_lock_for( const size_t timeout = 100 )
    {
      is_locked = mtx.try_lock_for( timeout );
      return is_locked;
    }

  private:
    mutex_type &mtx;
    bool is_locked;
  };

  /**
   *  Variant of the Lockable interface that doesn't depend on inheritance.
   *  This helps embedded systems to consume less memory for drivers.
   */

  /**
   * @brief CRTP interface to add a lock functionality to a class.
   *
   * @tparam T  Base class needing a lock interface
   */
  template<class T>
  class Lockable
  {
  public:
    void lock()
    {
      static_cast<T *>( this )->mClsMutex.lock();
    }

    void lockFromISR()
    {
      static_cast<T *>( this )->mClsMutex.lock();
    }

    bool try_lock_for( const size_t timeout )
    {
      return static_cast<T *>( this )->mClsMutex.try_lock_for( timeout );
    }

    void unlock()
    {
      static_cast<T *>( this )->mClsMutex.unlock();
    }

    void unlockFromISR()
    {
      static_cast<T *>( this )->mClsMutex.unlock();
    }

  protected:
    RecursiveTimedMutex mClsMutex;
  };


  /**
   * @brief CRTP interface to add AsyncIO notification functionality to a class
   *
   * @tparam T  Base class being extended
   */
  template<class T>
  class AsyncIO  // : public virtual AsyncIOInterface
  {
  public:
    AsyncIO() :
        mAIOAllowedEvents( 0xFFFFFFFF ), mAIOEvent( Chimera::Event::Trigger::UNKNOWN ), mAIOSignal( 1 )
    {
    }

    ~AsyncIO()
    {
    }


    Chimera::Status_t await( const Chimera::Event::Trigger event, const size_t timeout )
    {
      /*-----------------------------------------------------------------------
      Check for event support
      -----------------------------------------------------------------------*/
      size_t event_idx = static_cast<size_t>( event );
      if ( ( event >= Chimera::Event::Trigger::NUM_OPTIONS ) || !( mAIOAllowedEvents & ( 1u << event_idx ) ) )
      {
        return Chimera::Status::NOT_SUPPORTED;
      }

      /*-----------------------------------------------------------------------
      Wait for the event to occur. There are two timeouts at play:
        1. High level timeout to ensure no infinite loops.
        2. More fine-grained timeout to accurately block in "try_acquire_for()"
      -----------------------------------------------------------------------*/
      size_t startTime         = Chimera::millis();
      size_t lastWake          = startTime;
      size_t waitTimeRemaining = timeout;

      while ( ( Chimera::millis() - startTime ) < timeout )
      {
        mAIOSignal.try_acquire_for( waitTimeRemaining );

        /*---------------------------------------------------------------------
        Expected event (or error) signaled?
        ---------------------------------------------------------------------*/
        if ( ( mAIOEvent == event ) || ( mAIOEvent == Chimera::Event::Trigger::TRIGGER_SYSTEM_ERROR ) )
        {
          if ( mAIOEvent == Chimera::Event::Trigger::TRIGGER_SYSTEM_ERROR )
          {
            return Chimera::Status::FAIL;
          }
          else
          {
            return Chimera::Status::OK;
          }
        }

        /*---------------------------------------------------------------------
        Check for a timeout
        ---------------------------------------------------------------------*/
        size_t timeElapsed = Chimera::millis() - lastWake;
        if ( timeElapsed >= waitTimeRemaining )
        {
          return Chimera::Status::TIMEOUT;
        }

        /*---------------------------------------------------------------------
        Received a signal, but it was the wrong one. Keep moving forward.
        ---------------------------------------------------------------------*/
        lastWake = Chimera::millis();
        waitTimeRemaining -= timeElapsed;
      }
    }


    Chimera::Status_t await( const Chimera::Event::Trigger event, Chimera::Thread::BinarySemaphore &notifier,
                             const size_t timeout )
    {
      auto result = await( event, timeout );
      if ( result == Chimera::Status::OK )
      {
        notifier.release();
      }

      return result;
    }

  protected:
    uint32_t mAIOAllowedEvents;                   /**< Bit mask of events supported */

    /**
     * @brief Initialize the AsyncIO runtime
     *
     * This ensures the semaphore is at a known state that will block on first attempt
     * to wait for an event. Only call this method during the inheriting class init sequence.
     */
    void initAIO()
    {
      mAIOEvent = Chimera::Event::Trigger::UNKNOWN;
      mAIOSignal.try_acquire();
    }

    /**
     * @brief Lets the AsyncIO interface know an event has happened
     *
     * @param trigger
     */
    void signalAIO( const Chimera::Event::Trigger trigger )
    {
      mAIOEvent = trigger;
      mAIOSignal.release();
    }

  private:
    Chimera::Event::Trigger mAIOEvent;            /**< Which event was triggered by the class */
    Chimera::Thread::BinarySemaphore mAIOSignal;  /**< Lightweight semaphore to block on */
  };
}  // namespace Chimera::Thread

#endif /* !CHIMERA_THREADING_EXTENSIONS_HPP */
