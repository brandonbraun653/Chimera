/******************************************************************************
 *  File Name:
 *    timer_base_intf.hpp
 *
 *  Description:
 *    Interface classes for base time unit classes
 *
 *  2022-2023 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_BASE_TIMER_INTERFACES_HPP
#define CHIMERA_BASE_TIMER_INTERFACES_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/source/drivers/peripherals/timer/timer_common_types.hpp>
#include <Chimera/source/drivers/peripherals/timer/timer_intf.hpp>
#include <Chimera/source/drivers/peripherals/clock/clock_types.hpp>

namespace Chimera::Timer
{
  /*---------------------------------------------------------------------------
  Structures
  ---------------------------------------------------------------------------*/
  struct CoreConfig
  {
    Instance            instance;    /**< Peripheral instance being configured */
    Chimera::Clock::Bus clockSource; /**< Clock source driving the timer */
    float               baseFreq;    /**< Base clocking frequency of the timer in Hz */
    float               tolerance;   /**< Percent error allowed in base frequency: 0.0f-100.0f */

    void clear()
    {
      instance    = Instance::NOT_SUPPORTED;
      clockSource = Chimera::Clock::Bus::MAX_NUM_CLOCKS;
      baseFreq    = std::numeric_limits<float>::max();
      tolerance   = 5.0f;
    }
  };


  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  /**
   * @brief Core class that allows control of common basic timer attributes
   */
  class Core
  {
  public:
    /**
     * @brief Initialize the core timer
     *
     * @param cfg
     * @return Chimera::Status_t
     */
    Chimera::Status_t initCore( const CoreConfig &cfg );

    /**
     * @brief Enable the timer counter
     */
    void startCounter();

    /**
     * @brief Disable the timer counter
     */
    void stopCounter();
  };
}  // namespace Chimera::Timer

#endif /* !CHIMERA_BASE_TIMER_INTERFACES_HPP */
