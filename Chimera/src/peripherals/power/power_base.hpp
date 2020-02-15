/********************************************************************************
 *  File Name:
 *    power_base.hpp
 *
 *  Description:
 *    Abstract base class interface
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_POWER_BASE_HPP
#define CHIMERA_POWER_BASE_HPP

/* Chimera Includes */
#include <Chimera/src/peripherals/power/power_intf.hpp>

namespace Chimera::Power
{
  class InfoInterfaceDisabled : public InfoInterface
  {
  public:
    InfoInterfaceDisabled() = default;
    ~InfoInterfaceDisabled() = default;

    float systemVCC() final override
    {
      return 0.0f;
    }
  };  
}

#endif  /* !CHIMERA_POWER_BASE_HPP */
