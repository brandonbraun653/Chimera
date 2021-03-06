/********************************************************************************
 *  File Name:
 *    gpio_intf.hpp
 *
 *  Description:
 *    Models the Chimera GPIO interface
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_GPIO_INTERFACE_HPP
#define CHIMERA_GPIO_INTERFACE_HPP

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/exti>
#include <Chimera/function>
#include <Chimera/thread>
#include <Chimera/source/drivers/peripherals/gpio/gpio_types.hpp>

namespace Chimera::GPIO
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


  /*-------------------------------------------------------------------------------
  Classes
  -------------------------------------------------------------------------------*/
  /**
   * Defines expected behavior for all embedded systems that allow the user to control
   * GPIO pins. This is a pure virtual class.
   */
  class HWInterface
  {
  public:
    virtual ~HWInterface() = default;

    /**
     *  Initialize the GPIO object to use a specific port and pin assignment.
     *
     *	@param[in]	port        The port to be used
     *	@param[in]	pin         The pin to be used
     *	@return Chimera::Status_t
     *
     *
     *  |   Return Value   |             Explanation            |
     *  |:----------------:|:----------------------------------:|
     *  |               OK | The GPIO was initialized correctly |
     *  |             FAIL | The GPIO failed initialization     |
     *  | INVAL_FUNC_PARAM | User passed in an invalid value    |
     */
    virtual Chimera::Status_t init( const Chimera::GPIO::Port port, const uint8_t pin ) = 0;

    /**
     *  Initialize the GPIO object using the fully descriptive initialization struct
     *
     *	@param[in]	pinInit     Initialization struct
     *	@return Chimera::Status_t
     *
     *
     *  |   Return Value   |             Explanation            |
     *  |:----------------:|:----------------------------------:|
     *  |               OK | The GPIO was initialized correctly |
     *  |             FAIL | The GPIO failed initialization     |
     *  | INVAL_FUNC_PARAM | User passed in an invalid value    |
     */
    virtual Chimera::Status_t init( const Chimera::GPIO::PinInit &pinInit ) = 0;

    /**
     *  Change the GPIO pin electrical input/output behavior mode
     *
     *	@param[in]	drive       The new drive mode to be set
     *	@param[in]	pullup      Enable or disable pullups
     *	@return Chimera::Status_t
     *
     *	|   Return Value   |                 Explanation                 |
     *  |:----------------:|:-------------------------------------------:|
     *  |               OK | The pin set the mode and pull-ups correctly |
     *  |             FAIL | The pin failed applying the settings        |
     *  | INVAL_FUNC_PARAM | User passed in an invalid value             |
     *  |  NOT_INITIALIZED | The GPIO object has not been initialized    |
     */
    virtual Chimera::Status_t setMode( const Chimera::GPIO::Drive drive, const Chimera::GPIO::Pull pullup ) = 0;

    /**
     *  Change the logical state of the pin
     *
     *	@param[in]	state       The new state to transition into
     *	@return Chimera::Status_t
     *
     *	|   Return Value   |                 Explanation                 |
     *  |:----------------:|:-------------------------------------------:|
     *  |               OK | The pin set the new state correctly         |
     *  |             FAIL | The pin failed applying the settings        |
     *  |  NOT_INITIALIZED | The GPIO object has not been initialized    |
     */
    virtual Chimera::Status_t setState( const Chimera::GPIO::State state ) = 0;

    /**
     *  Read the current logical state of the pin
     *
     *	@param[out]	state       Variable to record the state info into
     *	@return Chimera::Status_t
     *
     *	|   Return Value  |                  Explanation                 |
     *  |:---------------:|:--------------------------------------------:|
     *  |              OK | The pin read the state correctly             |
     *  |            FAIL | The pin failed reading the state             |
     *  |   NOT_SUPPORTED | This behavior is not supported on the driver |
     *  | NOT_INITIALIZED | The GPIO object has not been initialized     |
     */
    virtual Chimera::Status_t getState( Chimera::GPIO::State &state ) = 0;

    /**
     *  Toggle the state of the pin
     *	@return Chimera::Status_t
     *
     *	|   Return Value  |                  Explanation                 |
     *  |:---------------:|:--------------------------------------------:|
     *  |              OK | The pin toggled correctly                    |
     *  |            FAIL | The pin failed toggling                      |
     *  |   NOT_SUPPORTED | This behavior is not supported on the driver |
     *  | NOT_INITIALIZED | The GPIO object has not been initialized     |
     */
    virtual Chimera::Status_t toggle() = 0;

    /**
     *  Attaches a function to be called when the GPIO pin is configured
     *  as an external interrupt source.
     *
     *  @param[in]  func        The function to be called
     *  @param[in]  trigger     Which edge to be triggered on
     *  @return Chimera::Status_t
     */
    virtual Chimera::Status_t attachInterrupt( Chimera::Function::vGeneric &func,
                                               const Chimera::EXTI::EdgeTrigger trigger ) = 0;

    /**
     *  Detaches any previously attached interrupt function
     *  @return void
     */
    virtual void detachInterrupt() = 0;

    /**
     *  Gets the EXTI interrupt line associated with the GPIO pin, assuming
     *  it has been properly configured.
     *
     *  @return Chimera::EXTI::EventLine_t
     */
    virtual Chimera::EXTI::EventLine_t getInterruptLine() = 0;
  };


  /**
   *  Virtual class to facilitate easy mocking of the driver
   */
  class IGPIO : virtual public HWInterface, virtual public Chimera::Thread::LockableInterface
  {
  public:
    virtual ~IGPIO() = default;
  };


  /**
   *  Concrete class declaration that promises to implement the virtual one, to
   *  avoid paying the memory penalty of a v-table lookup. Implemented project side
   *  using the Bridge pattern.
   */
  class Driver
  {
  public:
    Driver();
    ~Driver();

    /*-------------------------------------------------
    Interface: Hardware
    -------------------------------------------------*/
    Chimera::Status_t init( const Chimera::GPIO::PinInit &pinInit );
    Chimera::Status_t init( const Chimera::GPIO::Port port, const uint8_t pin );
    Chimera::Status_t setMode( const Chimera::GPIO::Drive drive, const Chimera::GPIO::Pull pull );
    Chimera::Status_t setState( const Chimera::GPIO::State state );
    Chimera::Status_t getState( Chimera::GPIO::State &state );
    Chimera::Status_t toggle();
    Chimera::Status_t attachInterrupt( Chimera::Function::vGeneric &func, const Chimera::EXTI::EdgeTrigger trigger );
    void detachInterrupt();
    Chimera::EXTI::EventLine_t getInterruptLine();

    /*-------------------------------------------------
    Interface: Lockable
    -------------------------------------------------*/
    void lock();
    void lockFromISR();
    bool try_lock_for( const size_t timeout );
    void unlock();
    void unlockFromISR();

  private:
    void *mDriver; /**< Instance of the implementer's GPIO driver */
  };

}  // namespace Chimera::GPIO

#endif /* !CHIMERA_GPIO_INTERFACE_HPP */
