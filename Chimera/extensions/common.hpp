/********************************************************************************
 *   File Name:
 *       common.hpp
 *
 *   Description:
 *       Generic extensions that can be applied to any embedded driver
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_EXTENSIONS_COMMON_HPP
#define CHIMERA_EXTENSIONS_COMMON_HPP

/* C++ Includes */
#include <cstdint>

/* Chimera Includes */
#include <Chimera/types/common_types.hpp>

namespace Chimera::Hardware
{
  class StateUpdator
  {
    virtual ~StateUpdator() = default;

    /**
     *  Gets the current execution state of the hardware. No realtime
     *  guarantees are made about the result of this function and all
     *  thread safety is left up to the implementer for minimal required
     *  overhead.
     * 
     *  @return Chimera::Hardware::Status
     */
    virtual Chimera::Hardware::Status executionState()
    {
      return Chimera::Hardware::Status::PERIPHERAL_UNAVAILABLE;
    }

    /**
     *  Queries the underlying hardware driver to see if the current 
     *  execution state is equal to the query. All thread safety is
     *  left up to the implementer to allow for minimal required overhead.
     * 
     *  @return bool
     */
    virtual bool executionState( const Chimera::Hardware::Status query )
    {
      return false;
    }

    //virtual uint8_t has_
  };
}  // namespace Chimera

#endif /* !CHIMERA_EXTENSIONS_COMMON_HPP */