/********************************************************************************
 *  File Name:
 *    timer_intf.hpp
 *
 *	Description:
 *    Chimera timer interface common to all peripherals
 *
 *  2020-2022 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_TIMER_INTERFACE_HPP
#define CHIMERA_TIMER_INTERFACE_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/event>
#include <Chimera/thread>
#include <Chimera/source/drivers/peripherals/timer/timer_common_types.hpp>

namespace Chimera::Timer
{
  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  namespace Backend
  {
    /**
     *  Registers the backend driver with Chimera
     *
     *  @param[in]  registry    Chimera's copy of the driver interface
     *  @return Chimera::Status_t
     */
    extern Chimera::Status_t registerDriver( DriverConfig &registry );
  }  // namespace Backend

  /*---------------------------------------------------------------------------
  Interface Classes
  ---------------------------------------------------------------------------*/
  /**
   * @brief Generic interface to a timer instance
   *
   */
  class ITimer
  {
  public:
    /**
     * @brief Virtual destructor to allow the use of dynamic_cast
     */
    virtual ~ITimer() = default;

    virtual void start() = 0;

    virtual void stop() = 0;
  };

  /**
   * @brief CRTP interface expected by all timer based functionality
   *
   * @tparam Derived    Class type being extended
   * @tparam ifType     Enum of timer hardware interface type
   * @tparam clsType    Enum of timer class object type
   */
  template<class Derived, CoreFeature ifType, TimerInterface clsType>
  class Introspection
  {
  public:

    constexpr CoreFeature getInterfaceType()
    {
      return ifType;
    }

    constexpr TimerInterface getClassType()
    {
      return clsType;
    }

  protected:
    // Shared data here? What's common to all timers?
  };

}  // namespace Chimera::Timer

#endif /* !CHIMERA_TIMER_INTERFACE_HPP */
