/********************************************************************************
 *   File Name:
 *    gpio_types.hpp
 *
 *   Description:
 *    Chimera GPIO types
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_GPIO_TYPES_HPP
#define CHIMERA_GPIO_TYPES_HPP

/* C++ Includes */
#include <cstdint>
#include <memory>

/* Chimera Includes */
#include <Chimera/types/common_types.hpp>

namespace Chimera
{
  namespace GPIO
  {
    class GPIOClass;
    using GPIOClass_sPtr = std::shared_ptr<Chimera::GPIO::GPIOClass>;
    using GPIOClass_uPtr = std::unique_ptr<Chimera::GPIO::GPIOClass>;

    class Status : public CommonStatusCodes
    {
    public:
      static constexpr Status_t codeOffset = 100;

      // Add status codes as needed. Currently all CommonStatusCodes suffice.
    };

    enum class Drive : uint8_t
    {
      INPUT,
      OUTPUT_PUSH_PULL,
      OUTPUT_OPEN_DRAIN,
      ALTERNATE_PUSH_PULL,
      ALTERNATE_OPEN_DRAIN,
      ANALOG,
      HIZ,

      NUM_DRIVES,
      UNKNOWN_DRIVE
    };

    enum class State : bool
    {
      HIGH = true,
      HI   = true,
      ON   = true,
      LOW  = false,
      LO   = false,
      OFF  = false
    };

    enum class Pull : uint8_t
    {
      NO_PULL,
      PULL_UP,
      PULL_DN,

      NUM_PULL_OPTIONS,
      UNKNOWN_PULL
    };

    /**
     *  GPIO register banks are typically grouped into ports, but there is no commonly accepted
     *  naming convention amongst all MCU manufacturers. As such, the 'PortX' nomenclature is 
     *  used to describe all GPIO IO banks with 'PortA' being the first bank and 'PortX' being
     *  the last bank.
     *
     *  @warning  The enum values are allowed to be used as array accessor indices, so do not start 
     *            assigning random values to the enum literals otherwise low level code could break.
     */
    enum class Port : uint8_t
    {
      PORTA = 0u,
      PORTB,
      PORTC,
      PORTD,
      PORTE,
      PORTF,
      PORTG,
      PORTH,
      PORTI,
      PORTJ,
      PORTK,
      PORTL,

      NUM_PORTS,
      UNKNOWN_PORT
    };

    struct PinInit
    {
      Pull pull                                = Pull::UNKNOWN_PULL; /**< Pull-up/down configuration */
      Port port                                = Port::UNKNOWN_PORT; /**< Pin IO port */
      Drive drive                              = Drive::INPUT;       /**< Pin IO drive type */
      State state                              = State::LOW;         /**< Default logical pin state on init */
      uint8_t pin                              = 0u;                 /**< Pin number on the given port */
      uint32_t alternate                       = 0u;                 /**< Project specific indicator of pin AF config */
      Chimera::Hardware::AccessMode accessMode = Chimera::Hardware::AccessMode::BARE_METAL;
    };

  }  // namespace GPIO
}  // namespace Chimera

#endif /* !CHIMERA_GPIO_TYPES_HPP */