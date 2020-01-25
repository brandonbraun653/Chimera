/********************************************************************************
 *   File Name:
 *    threading_base.hpp
 *
 *   Description:
 *    Abstract base class interfaces
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_THREADING_ABSTRACT_BASE_HPP
#define CHIMERA_THREADING_ABSTRACT_BASE_HPP

/* Chimera Includes */
#include <Chimera/interface/threading_intf.hpp>

namespace Chimera::Threading
{
  class LockableUnsupported : public LockableInterface
  {
  public:
    LockableUnsupported()  = default;
    ~LockableUnsupported() = default;

    Chimera::Status_t lock( const size_t timeout_mS ) override
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    Chimera::Status_t lockFromISR( const size_t timeout_mS ) override
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    Chimera::Status_t unlock() final override
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    Chimera::Status_t unlockFromISR() final override
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }
  };

  class AsyncIOUnsupported : public AsyncIOInterface
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