/********************************************************************************
 *  File Name:
 *    timer_intf.hpp
 *
 *	 Description:
 *    Chimera timer interface
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_TIMER_INTERFACE_HPP
#define CHIMERA_TIMER_INTERFACE_HPP

/* Aurora Includes */
#include <Aurora/units/units.hpp>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/event>
#include <Chimera/thread>
#include <Chimera/src/peripherals/timer/timer_types.hpp>

namespace Chimera::Timer
{
  namespace Backend
  {
    /**
     *  Registers the backend driver with Chimera
     *  
     *  @param[in]  registry    Chimera's copy of the driver interface
     *  @return Chimera::Status_t
     */
    extern Chimera::Status_t registerDriver( DriverRegistration &registry );
  }

  class HardwareDriverInterface
  {
  public:
    virtual ~HardwareDriverInterface() = default;

    /*------------------------------------------------
    Configuration & Control
    ------------------------------------------------*/
    /**
     *  Initialize the entire timer peripheral attached to the channel given 
     *  in the configuration struct. 
     *
     *  @warning This will affect all channels attached to the peripheral
     *
     *  @param[in]  cfg   Configuration data
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t initPeripheral( const Chimera::Timer::DriverConfig &cfg ) = 0;

    /**
     *  Enables any behavior configured for this channel
     *
     *  @return void
     */
    virtual void enable() = 0;

    /**
     *  Disables any behavior configured for this channel.
     *  
     *  @return void
     */
    virtual void disable() = 0;

    /**
     *  Enables interrupt functionality for this timer channel
     *
     *  @param[in]  type    The ISR type to enable
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t enableISR( const Chimera::Timer::ISREvent type ) = 0;

    /**
     *  Disables interrupt functionality for this timer channel
     *
     *  @param[in]  type    The ISR type to disable
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t disableISR( const Chimera::Timer::ISREvent type ) = 0;

    /**
     *  Configures the channel for input capture behavior
     *  
     *  @param[in]  cfg   Configuration data
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t setupInputCapture( const Chimera::Timer::InputCapture::Config &cfg ) = 0;

    /**
     *  Configures the channel for output compare behavior
     *
     *  @param[in]  cfg   Configuration data
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t setupOutputCompare( const Chimera::Timer::OutputCompare::Config &cfg ) = 0;

    /**
     *  Configures the channel for pwm generation behavior
     *
     *  @param[in]  cfg   Configuration data
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t setupPWMGeneration( const Chimera::Timer::PWMGeneration::Config &cfg ) = 0;

    /**
     *  Configures the channel for one pulse behavior
     *
     *  @param[in]  cfg   Configuration data
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t setupOnePulse( const Chimera::Timer::OnePulse::Config &cfg ) = 0;

    /*------------------------------------------------
    Data getters
    ------------------------------------------------*/
    /**
     *  Gets the number of bits used in the up/down counter
     *
     *  @return size_t
     */
    virtual size_t counterBitWidth() = 0;

    /**
     *  Calculates the amount of time elapsed for each tick of the 
     *  timer given its current configuration.
     *
     *  @param[in]  units   The units to return the time in
     *  @return size_t
     */
    virtual size_t tickRate( const Chimera::Units::Time units ) = 0;

    /**
     *  Calculates the maximum amount of time that can elapse and the 
     *  timing functionality still be accurate.
     *  
     *  @param[in]  units   The units to return the time in
     *  @return size_t
     */
    virtual size_t maxPeriod( const Chimera::Units::Time units ) = 0;

    /**
     *  Calculates the minimum amount of time that can elapse and the
     *  timinig functionality still be accurate.
     *
     *  @param[in]  units   The units to return the time in
     *  @return size_t
     */
    virtual size_t minPeriod( const Chimera::Units::Time units ) = 0;

    /**
     *  Gets the current operational mode of the timer channel
     *
     *  @return Chimera::Timer::Mode
     */
    virtual Chimera::Timer::Mode currentMode() = 0;
  };


  class ITimer : virtual public HardwareDriverInterface,
                 virtual public Chimera::Event::ListenerInterface,
                 virtual public Chimera::Threading::LockableInterface
  {
  public:
    virtual ~ITimer() = default;
  };
}  // namespace Chimera::Timer

#endif  /* !CHIMERA_TIMER_INTERFACE_HPP */
