/********************************************************************************
 *   File Name:
 *    power_intf.hpp
 *
 *   Description:
 *    Models the Chimera power interface 
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/


#pragma once 
#ifndef CHIMERA_POWER_INTERFACE_HPP
#define CHIMERA_POWER_INTERFACE_HPP

namespace Chimera
{
  namespace Power
  {
    class InfoInterface
    {
    public:
      
      virtual ~InfoInterface() = default;

      virtual float systemVCC() = 0;
      
    };

    class InfoInterfaceDisabled : InfoInterface
    {
    public:
      float systemVCC() final override
      {
        return 0.0f;
      }
    };


  }  // namespace Power
}  // namespace Chimera

#endif /* !CHIMERA_POWER_INTERFACE_HPP */