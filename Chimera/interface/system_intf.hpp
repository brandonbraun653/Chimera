/********************************************************************************
 *   File Name:
 *    system_intf.hpp
 *
 *   Description:
 *    Models the Chimera system interface
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_SYSTEM_INTERFACE_HPP
#define CHIMERA_SYSTEM_INTERFACE_HPP

/* C++ Includes */
#include <cstdint>

/* Chimera Includes */
#include <Chimera/types/common_types.hpp>
#include <Chimera/types/system_types.hpp>

namespace Chimera::System
{
  /**
   *  Low level driver specific system initialization function used for operations
   *  like clock configuration, periphal startup, etc. 
   *
   *  @note     Intended to allow the backend driver to initialize itself before Chimera
   *            code begins execution.
   *
   *  @warning  Chimera assumes that once this function exits, the backend driver is ready
   *            to be used in its entirety.
   *
   *  @return Chimera::Status_t
   */
  extern Chimera::Status_t prjSystemStartup();

  class ControlInterface
  {
  public:
    virtual Status_t reasonForReset( ResetType &reason ) = 0;

  private:
  };

  class ControlUnsupported : public ControlInterface
  {
  public:
    Status_t reasonForReset( ResetType &reason ) final override
    {
      reason = ResetType::UNKNOWN_RESET;
      return Chimera::CommonStatusCodes::NOT_SUPPORTED;
    }
  };


  class IdentifierInterface
  {
  public:
    virtual ~IdentifierInterface() = default;

    virtual uint32_t deviceID() = 0;

    virtual uint32_t uniqueID() = 0;
  };

  class IdentifierUnsupported : public IdentifierInterface
  {
  public:
    IdentifierUnsupported() = default;

    uint32_t deviceID() final override
    {
      return 0u;
    }

    uint32_t uniqueID() final override
    {
      return 0u;
    }
  };

}  // namespace Chimera::System

#endif /* !CHIMERA_SYSTEM_INTERFACE_HPP */