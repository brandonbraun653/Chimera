/********************************************************************************
 *   File Name:
 *    power.hpp
 *
 *   Description:
 *    Models the system power interface 
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/


#pragma once 
#ifndef CHIMERA_POWER_TYPES_HPP
#define CHIMERA_POWER_TYPES_HPP

#include <memory>

namespace Chimera
{
  namespace Power
  {
    class Info;
    
    using Info_sPtr = std::shared_ptr<Chimera::Power::Info>;
    using Info_uPtr = std::shared_ptr<Chimera::Power::Info>;
    
  }  // namespace Power
}  // namespace Chimera

#endif /* !CHIMERA_POWER_TYPES_HPP */