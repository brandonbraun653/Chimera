/********************************************************************************
 *  File Name:
 *    timer_intf.hpp
 *
 *	Description:
 *    Chimera timer interface common to all peripherals
 *
 *  Design Notes:
 *    While figuring out the interface for all timers, it quickly became apparent
 *    that they do a LOT of things. For example, on STM32L4 chips, there are no
 *    less than four types of timers, each supporting close to twenty various HW
 *    functionalities. You might say "well surely some of those are specific to
 *    STM32 right?", and you would be correct, but there is a surprisingly large
 *    number of functionalities which you would expect to be common across a wide
 *    range of MCUs from vendors far and wide.
 *
 *    What I'm getting at is the architectural nature of Chimera (inherit from
 *    and implement pure virtual base classes to guarantee consistent driver
 *    interfaces) does not scale well when you need dozens of functions just to
 *    cover a small subset of functionality. Embedded systems don't usually have
 *    much memory to begin with, so the virtual tables associated with these
 *    interfaces **cannot** be too large. It's self-defeating.
 *
 *    Usually in software it's a great idea to break things down into small pieces
 *    and not do too many things in one function. Due to how many features can
 *    be implemented with Timer hardware, combined with the memory constraints of
 *    an embedded system, a virtual interface was deemed not feasible. During
 *    initial development, this driver's vtable alone was consuming nearly 8kB.
 *    That's unnacceptable.
 *
 *    Several alternative architectural options were considered:
 *      1. CRTP (Curiously Recurring Template Pattern):
 *          Useful, but not if we don't want Chimera to know about the types
 *          present in the back end driver. Chimera should have no clue that the
 *          back end even exists. It's just an interface spec and data conduit.
 *
 *      2. C-Style Function Pointers:
 *          This would totally work, except it goes against Chimera's architectural
 *          model of having drivers as an object. The point of the Chimera project
 *          was to get away from C-style development and move into a more object
 *          oriented model.
 *
 *      3. Template MetaProgramming:
 *          Again, could totally work, but Chimera can't know about the back end
 *          driver types. "But what about type erasure?" Sure, if we wanted to
 *          pay the penalty of a large number of virtual functions again.
 *          (Hint: No) Chimera needs to work on small microcontrollers and the
 *          timer peripheral is a pretty core piece of hardware.
 *
 *    Ultimately the decision was made to go with something similar to a dispatcher.
 *    The timer interface will describe a few functions that allow generic behavior
 *    to be invoked by the back end driver via enumerated actions. It will be up
 *    to the back end to provide the expected functionality attached to that action
 *    enumeration. Data associated with these actions will be passed back and forth
 *    via callbacks and various data structures. This **should** allow for the full
 *    set of timer behaviors to be captured while keeping the memory overhead
 *    relatively minimal and the interface extensible in the future. Due to the fact
 *    that generally a timer will get encapsulated as part of another feature (PWM,
 *    scheduled callbacks, measurement, etc), it feels less important to have a more
 *    traditional peripheral interface and instead focus on making sure all features
 *    can be accessed. The many-to-one problem is difficult.
 *
 *    While the interface enforcement is a bit looser than desired (it relies on the
 *    programmer to follow the documented spec per action type), it gets the job done.
 *    None of the other Chimera interfaces behave in this manner, so it will be
 *    interesting to see how this plays out in practice.
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
  }  // namespace Backend

  /**
   *  Interface that interacts with the core timer peripheral via a dispatching mechanism.
   *  Various actions can be specified and the implementer can decide whether or not it's
   *  going to support the functionality.
   */
  class ITimerHardwareDriver
  {
  public:
    virtual ~ITimerHardwareDriver() = default;

    /*------------------------------------------------
    Configuration & Control
    ------------------------------------------------*/
    /**
     *  Initialization function for a core hardware feature that is supported
     *  by the peripheral. This should only do the bare minimum to configure
     *  the hardware.
     *
     *  @param[in]  feature   The hardware feature to be initialized
     *  @param[in]  init      Configuration data for the feature
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t initializeCoreFeature( const Chimera::Timer::CoreFeature feature,
                                                     Chimera::Timer::CoreFeatureInit &init ) = 0;

    /**
     *  Invoke some kind of behavior on the hardware. Usually this is to update previously configured
     *  hardware with new settings. For example, the PWM compare match value can be updated to
     *  change the output pulse width or frequency.
     *
     *  @param[in]  action    The desired to action to invoke
     *  @param[in]  arg       Data associated with the action
     *  @param[in]  argSize   Size of the data passed in
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t invokeAction( const Chimera::Timer::DriverAction action, void *arg, const size_t argSize ) = 0;

    /**
     *  Turn something in the hardware on, off, or another state
     *
     *  @param[in]  device    Hardware to interact with
     *  @param[in]  state     The state to place the hardware in
     */
    virtual Chimera::Status_t setState( const Chimera::Timer::Switchable device,
                                        const Chimera::Timer::SwitchableState state ) = 0;

    /*------------------------------------------------
    Data getters
    ------------------------------------------------*/
    /**
     *  Requests some data from the hardware. This could be taking a measurement, getting the results
     *  of a previous action, etc.
     *
     *  @param[in]  data      The data type that is being requested
     *  @param[out] arg       Memory for the data to be returned into
     *  @param[in]  argSize   Size of the output data buffer
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t requestData( const Chimera::Timer::DriverData data, void *arg, const size_t argSize ) = 0;

    /**
     *  Gets description information about the timer peripheral. This includes
     *  things like its current state, what its capabilities are, etc.
     *
     *  @return const Chimera::Timer::Descriptor *
     */
    virtual const Chimera::Timer::Descriptor *getDeviceInfo() = 0;
  };

  class ITimer : virtual public ITimerHardwareDriver, virtual public Chimera::Threading::LockableInterface
  {
  public:
    virtual ~ITimer() = default;
  };
}  // namespace Chimera::Timer

#endif /* !CHIMERA_TIMER_INTERFACE_HPP */
