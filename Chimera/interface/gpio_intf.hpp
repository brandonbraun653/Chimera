/********************************************************************************
 *   File Name:
 *    gpio_intf.hpp
 *
 *   Description:
 *    Models the Chimera GPIO interface 
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_GPIO_INTERFACE_HPP
#define CHIMERA_GPIO_INTERFACE_HPP

/* Chimera Includes */
#include <Chimera/types/common_types.hpp>
#include <Chimera/types/gpio_types.hpp>
#include <Chimera/threading.hpp>

namespace Chimera
{
  namespace GPIO
  {
    /**
     * Defines expected behavior for all embedded systems that allow the user to control
     * GPIO pins. This is a pure virtual/abstract class.
     */
    class Interface : public Threading::Lockable
    {
    public:
      /**
       *	Virtual destructor necessary for GMock as well as inheritors
       */
      virtual ~Interface() = default;

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
      virtual Chimera::Status_t setMode( const Chimera::GPIO::Drive drive, const bool pullup ) = 0;

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
       *
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
    };

    /**
     * Defines a real GPIO object that has completely disabled functionality. Used to allow code
     * to compile, but still fail at runtime in a clean manner.
     */
    class GPIOUnsupported : public Interface
    {
    public:
      GPIOUnsupported()  = default;
      ~GPIOUnsupported() = default;

      Chimera::Status_t init( const Chimera::GPIO::Port port, const uint8_t pin ) final override
      {
        return Chimera::GPIO::Status::FAIL;
      }

      Chimera::Status_t setMode( const Chimera::GPIO::Drive drive, const bool pullup ) final override
      {
        return Chimera::GPIO::Status::NOT_INITIALIZED;
      }

      Chimera::Status_t setState( const Chimera::GPIO::State state ) final override
      {
        return Chimera::GPIO::Status::NOT_INITIALIZED;
      }

      Chimera::Status_t getState( Chimera::GPIO::State &state ) final override
      {
        return Chimera::GPIO::Status::NOT_INITIALIZED;
      }

      Chimera::Status_t toggle() final override
      {
        return Chimera::GPIO::Status::NOT_INITIALIZED;
      }
    };

  }  // namespace GPIO
}

#endif /* !CHIMERA_GPIO_INTERFACE_HPP */
