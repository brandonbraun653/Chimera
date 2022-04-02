/******************************************************************************
*  File Name:
*    timer_base_intf.hpp
*
*  Description:
*    Interface classes for base time unit classes
*
*  2022 | Brandon Braun | brandonbraun653@protonmail.com
*****************************************************************************/

#pragma once
#ifndef CHIMERA_BASE_TIMER_INTERFACES_HPP
#define CHIMERA_BASE_TIMER_INTERFACES_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/source/drivers/peripherals/timer/timer_common_types.hpp>
#include <Chimera/source/drivers/peripherals/timer/timer_intf.hpp>

namespace Chimera::Timer
{
  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  /**
   * @brief Core class that allows control of common basic timer attributes
   */
  class Core
  {
  public:

    Chimera::Status setTickFreq( const float freq );

  };
}  // namespace Chimera::Timer

#endif  /* !CHIMERA_BASE_TIMER_INTERFACES_HPP */
