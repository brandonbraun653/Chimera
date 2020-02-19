/********************************************************************************
 *  File Name:
 *    gpio_types.hpp
 *
 *  Description:
 *    Chimera GPIO types
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_GPIO_TYPES_HPP
#define CHIMERA_GPIO_TYPES_HPP

/* STL Includes */
#include <cstdint>
#include <memory>

/* Chimera Includes */
#include <Chimera/common>

namespace Chimera::GPIO
{
  using Pin = uint8_t;

  class Status : public CommonStatusCodes
  {
  public:
    static constexpr Status_t codeOffset = Chimera::Status::Internal::gpioOffset;

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
    UNKNOWN_DRIVE,
    NUM_OPTIONS
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
    UNKNOWN_PULL,
    NUM_OPTIONS
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

    UNKNOWN_PORT,
    NUM_OPTIONS
  };

  struct PinInit
  {
    Pull pull;          /**< Pull-up/down configuration */
    Port port;          /**< Pin IO port */
    Drive drive;        /**< Pin IO drive type */
    State state;        /**< Default logical pin state on init */
    Pin pin;            /**< Pin number on the given port */
    uint32_t alternate; /**< Project specific indicator of pin AF config */
    Chimera::Hardware::AccessMode accessMode;
    bool validity; /**< Indicates if the config should be initialized */

    void clear()
    {
      pull       = Pull::UNKNOWN_PULL;
      port       = Port::UNKNOWN_PORT;
      drive      = Drive::INPUT;
      state      = State::LOW;
      pin        = 0u;
      alternate  = 0u;
      accessMode = Chimera::Hardware::AccessMode::BARE_METAL;
      validity   = false;
    }
  };


  class IGPIO;

  using GPIO_sPtr = std::shared_ptr<IGPIO>;
  using GPIO_uPtr = std::unique_ptr<IGPIO>;

  namespace Backend
  {
    using Initialize_FPtr         = Chimera::Status_t ( * )( void );
    using Reset_FPtr              = Chimera::Status_t ( * )( void );
    using CreateSharedObject_FPtr = GPIO_sPtr ( * )( void );
    using CreateUniqueObject_FPtr = GPIO_uPtr ( * )( void );

    struct DriverConfig
    {
      bool isSupported; /**< A simple flag to let Chimera know if the driver is supported */

      /**
       *  Function pointer that initializes the backend driver's
       *  memory. Should really only call once for initial set up.
       */
      Initialize_FPtr initialize;

      /**
       *  Resets the backend driver hardware to default configuration
       *  settings, but does not wipe out any memory.
       */
      Reset_FPtr reset;

      /**
       *  Factory function that creates a shared_ptr instance of the backend
       *  driver, as long as it conforms to the expected interface.
       */
      CreateSharedObject_FPtr createShared;

      /**
       *  Factory function that creates a unique_ptr instance of the backend
       *  driver, as long as it conforms to the expected interface.
       */
      CreateUniqueObject_FPtr createUnique;
    };

  }  // namespace Backend
}  // namespace Chimera::GPIO

#endif /* !CHIMERA_GPIO_TYPES_HPP */
