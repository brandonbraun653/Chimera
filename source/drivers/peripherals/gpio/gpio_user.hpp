/******************************************************************************
 *  File Name:
 *    gpio.hpp
 *
 *  Description:
 *    Implements an interface to create a Chimera GPIO peripheral
 *
 *  2019-2022 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_GPIO_HPP
#define CHIMERA_GPIO_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/source/drivers/peripherals/gpio/gpio_intf.hpp>

namespace Chimera::GPIO
{
  /*---------------------------------------------------------------------------
  Public Functions
  ---------------------------------------------------------------------------*/
  Chimera::Status_t initialize();
  Chimera::Status_t reset();
  Driver_rPtr       getDriver( const Port port, const Pin pin );

  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  class Driver : public Chimera::Thread::Lockable<Driver>, public IGPIO
  {
  public:
    Driver();
    ~Driver();

    /**
     *  Initialize the GPIO object to use a specific port and pin assignment.
     *
     *  @param    port        The port to be used
     *  @param    pin         The pin to be used
     *  @return Chimera::Status_t
     */
    Chimera::Status_t init( const Chimera::GPIO::Port port, const uint8_t pin );

    /**
     *  Initialize the GPIO object using the fully descriptive initialization struct
     *
     *  @param    pinInit     Initialization struct
     *  @return Chimera::Status_t
     */
    Chimera::Status_t init( const Chimera::GPIO::PinInit &pinInit );

    /**
     *  Change the GPIO pin electrical input/output behavior mode
     *
     *  @param    drive       The new drive mode to be set
     *  @param    pullup      Enable or disable pullups
     *  @return Chimera::Status_t
     */
    Chimera::Status_t setMode( const Chimera::GPIO::Drive drive, const Chimera::GPIO::Pull pullup );

    /**
     *  Change the logical state of the pin
     *
     *  @param    state       The new state to transition into
     *  @return Chimera::Status_t
     */
    Chimera::Status_t setState( const Chimera::GPIO::State state );

    /**
     *  Read the current logical state of the pin
     *
     *  @param    state       Variable to record the state info into
     *  @return Chimera::Status_t
     */
    Chimera::Status_t getState( Chimera::GPIO::State &state );

    /**
     *  Toggle the state of the pin
     *  @return Chimera::Status_t
     */
    Chimera::Status_t toggle();

    /**
     *  Attaches a function to be called when the GPIO pin is configured
     *  as an external interrupt source.
     *
     *  @param    func        The function to be called
     *  @param    trigger     Which edge to be triggered on
     *  @return Chimera::Status_t
     */
    Chimera::Status_t attachInterrupt( Chimera::Function::vGeneric &func, const Chimera::EXTI::EdgeTrigger trigger );

    /**
     *  Detaches any previously attached interrupt function
     *  @return void
     */
    void detachInterrupt();

    /**
     *  Gets the EXTI interrupt line associated with the GPIO pin, assuming
     *  it has been properly configured.
     *
     *  @return Chimera::EXTI::EventLine_t
     */
    Chimera::EXTI::EventLine_t getInterruptLine();

  private:
    friend Chimera::Thread::Lockable<Driver>;
    void *mImpl; /**< Implementation details of the driver */
  };

}  // namespace Chimera::GPIO

#endif /* !CHIMERA_GPIO_HPP */
