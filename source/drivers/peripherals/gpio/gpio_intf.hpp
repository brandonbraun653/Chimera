/********************************************************************************
 *  File Name:
 *    gpio_intf.hpp
 *
 *  Description:
 *    Models the Chimera GPIO interface
 *
 *  2019-2022 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_GPIO_INTERFACE_HPP
#define CHIMERA_GPIO_INTERFACE_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
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
    virtual Chimera::Status_t init( const Chimera::GPIO::Port port, const uint8_t pin ) = 0;
    virtual Chimera::Status_t init( const Chimera::GPIO::PinInit &pinInit ) = 0;
    virtual Chimera::Status_t setMode( const Chimera::GPIO::Drive drive, const Chimera::GPIO::Pull pullup ) = 0;
    virtual Chimera::Status_t setState( const Chimera::GPIO::State state ) = 0;
    virtual Chimera::Status_t getState( Chimera::GPIO::State &state ) = 0;
    virtual Chimera::Status_t toggle() = 0;
    virtual Chimera::Status_t attachInterrupt( Chimera::Function::vGeneric &func,
                                               const Chimera::EXTI::EdgeTrigger trigger ) = 0;
    virtual void detachInterrupt() = 0;
    virtual Chimera::EXTI::EventLine_t getInterruptLine() = 0;
  };


  /**
   *  Virtual class to facilitate easy mocking of the driver
   */
#if defined( CHIMERA_VIRTUAL )
  class IGPIO : virtual public HWInterface, virtual public Chimera::Thread::LockableInterface
  {
  public:
    virtual ~IGPIO() = default;
  };
#else
  class IGPIO
  {
  };
#endif /* CHIMERA_VIRTUAL */

}  // namespace Chimera::GPIO

#endif /* !CHIMERA_GPIO_INTERFACE_HPP */
