/********************************************************************************
 *  File Name:
 *    threading_base.hpp
 *
 *  Description:
 *    Abstract base class interfaces
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_THREADING_ABSTRACT_BASE_HPP
#define CHIMERA_THREADING_ABSTRACT_BASE_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/src/event/event_types.hpp>
#include <Chimera/src/threading/semaphore.hpp>

namespace Chimera::Threading
{
  class LockableUnsupported : virtual public LockableInterface
  {
  public:
    LockableUnsupported()  = default;
    ~LockableUnsupported() = default;

    void lock()  override
    {
    }

    void lockFromISR()  override
    {
    }

    bool try_lock_for( const size_t timeout )  override
    {
      return false;
    }

    void unlock()  override
    {
    }

    void unlockFromISR()  override
    {
    }
  };

  class AsyncIOUnsupported : virtual public AsyncIOInterface
  {
  public:
    AsyncIOUnsupported()  = default;
    ~AsyncIOUnsupported() = default;

    Chimera::Status_t await( const Chimera::Event::Trigger event, const size_t timeout ) final override
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    Chimera::Status_t await( const Chimera::Event::Trigger event, Chimera::Threading::BinarySemaphore &notifier,
                             const size_t timeout ) final override
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }
  };

}  // namespace Chimera::Threading

#endif /* !CHIMERA_THREADING_ABSTRACT_BASE_HPP */