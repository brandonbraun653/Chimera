/********************************************************************************
 *   File Name:
 *    event_base.hpp
 *
 *   Description:
 *    Abstract base class interfaces
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_EVENT_ABSTRACT_BASE_HPP
#define CHIMERA_EVENT_ABSTRACT_BASE_HPP

/* STL Includes */
#include <cstdint>

/* Chimera Includes*/
#include <Chimera/common>
#include <Chimera/src/event/event_intf.hpp>
#include <Chimera/src/event/event_types.hpp>

namespace Chimera::Event
{
  class ListenerUnsupported : virtual public ListenerInterface
  {
  public:
    ListenerUnsupported()  = default;
    ~ListenerUnsupported() = default;

    Chimera::Status_t registerListener( Actionable &listener, const size_t timeout, size_t &registrationID ) final override
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }

    Chimera::Status_t removeListener( const size_t registrationID, const size_t timeout ) final override
    {
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }
  };
}

#endif /* !CHIMERA_EVENT_ABSTRACT_BASE_HPP */