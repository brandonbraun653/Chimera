/********************************************************************************
 *  File Name:
 *    clock_intf.hpp
 *
 *  Description:
 *    Provides methods used to interact with the system clocks.
 * 
 *  Notes:
 *    The vast majority of these functions must be implemented in the backend 
 *    driver for the given processor. Without this, the system will have no clue
 *    how to actual perform these actions.
 * 
 *    No assumptions are made about the thread safety of the implementation. If 
 *    you are concerned, place the calls in a critical section.
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_CLOCK_INTERFACE_HPP
#define CHIMERA_CLOCK_INTERFACE_HPP

/* STL Includes */
#include <cstdint>

/* Chimera Includes */
#include <Chimera/types/clock_types_fwd.hpp>
#include <Chimera/common>
#include <Chimera/types/peripheral_types.hpp>

namespace Chimera::Clock
{
  /**
   *  Initialize the Clock driver resources. Must be called before 
   *  any other functionality is used.
   *
   *  @return Chimera::Status_t
   */
  extern Chimera::Status_t initializeDriver();

  /**
   *  Checks if the clock resources have been initialized and is ready
   *  for use. This does not mean the clock subsystem is fully configured
   *  and ready per user configuration.
   *
   *  @see initialize()
   *
   *  @return bool
   */
  extern bool isDriverInitialized();


  /**
   *  Turns on the clock to the given peripheral. The actual clock
   *  source must be configured before this function is called.
   * 
   *  @param[in]  periph    The peripheral to turn on the clock for
   *  @return Chimera::Status_t
   * 
   *  |  Return Value |             Explanation            |
   *  |:-------------:|:----------------------------------:|
   *  |            OK | The clock was successfully enabled |
   *  |          FAIL | The clock could not be enabled     |
   *  | NOT_SUPPORTED | This peripheral is not supported   |
   */
  Chimera::Status_t periphEnable( const Chimera::Peripheral::Type periph );

  /**
   *  Turns off the clock to the given peripheral.
   *  
   *  @param[in]  periph    The peripheral to turn off the clock for
   *  @return Chimera::Status_t
   *  
   *  |  Return Value |             Explanation             |
   *  |:-------------:|:-----------------------------------:|
   *  |            OK | The clock was successfully disabled |
   *  |          FAIL | The clock could not be disabled     |
   *  | NOT_SUPPORTED | This peripheral is not supported    |
   */
  Chimera::Status_t periphDisable( const Chimera::Peripheral::Type periph );

  /**
   *  Enables the given clock bus
   * 
   *  @param[in]  bus       The clock bus to be turned on
   *  @return Chimera::Status_t
   * 
   *  |  Return Value |             Explanation            |
   *  |:-------------:|:----------------------------------:|
   *  |            OK | The clock was successfully enabled |
   *  |          FAIL | The clock could not be enabled     |
   *  | NOT_SUPPORTED | This clock is not supported        |
   */
  Chimera::Status_t enableClock( const Chimera::Clock::Bus bus );

  /**
   *  Disables the given clock bus
   * 
   *  @param[in]  bus       The clock bus to be turned off
   *  @return Chimera::Status_t
   * 
   *  |  Return Value |             Explanation             |
   *  |:-------------:|:-----------------------------------:|
   *  |            OK | The clock was successfully disabled |
   *  |          FAIL | The clock could not be disabled     |
   *  | NOT_SUPPORTED | This clock is not supported         |
   */
  Chimera::Status_t disableClock( const Chimera::Clock::Bus bus );

  /**
   *  Checks if the given bus has its clock enabled
   * 
   *  @param[in]  bus       The clock bus to check
   *  @return bool
   */
  bool isEnabled( const Chimera::Clock::Bus bus );

  /**
   *  Gets the current operating frequency of the clock in Hz
   *  
   *  @param[in]  bus       The clock bus to check
   *  @return size_t
   */
  size_t getFrequency( const Chimera::Clock::Bus bus );

  /**
   *  Sets the operating frequency of a clock bus in Hz.
   * 
   *  @note This function attempts to exactly match the requested frequency.
   *        For a best-fit behavior, use setFrequencyImprecise().
   *  
   *  @param[in]  bus       The clock bus to update the frequency on
   *  @param[in]  freq      The desired frequency in Hz
   *  @return Chimera::Status_t
   * 
   *  |  Return Value |             Explanation            |
   *  |:-------------:|:----------------------------------:|
   *  |            OK | The clock was successfully updated |
   *  |          FAIL | The clock failed to be updated     |
   *  | NOT_SUPPORTED | The clock is not supported         |
   */
  Chimera::Status_t setFrequency( const Chimera::Clock::Bus bus, const size_t freq );
  
  /**
   *  Sets the operating frequency of a clock bus in Hz. If the exact value
   *  cannot be achieved, the next best fit will be used instead. This could 
   *  be higher or lower than the requested value.
   * 
   *  @note Use getFrequency() to check the actual value of the clock after 
   *        this function completes successfully.
   *  
   *  @param[in]  bus       The clock bus to update the frequency on
   *  @param[in]  freq      The desired frequency in Hz
   *  @return Chimera::Status_t
   * 
   *  |  Return Value |             Explanation            |
   *  |:-------------:|:----------------------------------:|
   *  |            OK | The clock was successfully updated |
   *  |          FAIL | The clock failed to be updated     |
   *  | NOT_SUPPORTED | The clock is not supported         |
   */
  Chimera::Status_t setFrequencyImprecise( const Chimera::Clock::Bus bus, const size_t freq );

}  // namespace Chimera::Clock

#endif /* !CHIMERA_CLOCK_INTERFACE_HPP */