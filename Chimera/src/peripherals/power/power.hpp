/********************************************************************************
 *  File Name:
 *    power.hpp
 *
 *  Description:
 *    Models the system power interface
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_POWER_HPP
#define CHIMERA_POWER_HPP

/* STL Includes */
#include <memory>

/* Chimera Includes */
#include <Chimera/src/peripherals/power/power_intf.hpp>

namespace Chimera::Power
{
  using Info_sPtr = std::shared_ptr<InfoInterface>;
  using Info_uPtr = std::unique_ptr<InfoInterface>;

  Chimera::Status_t initialize();

  Info_sPtr create_shared_info_ptr();

  Info_uPtr create_unique_info_ptr();

}  // namespace Chimera::Power

#endif /* !CHIMERA_POWER_HPP */