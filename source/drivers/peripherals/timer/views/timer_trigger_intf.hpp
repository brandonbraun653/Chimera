/******************************************************************************
 *  File Name:
 *    timer_trigger_intf.hpp
 *
 *  Description:
 *    Timer view for acting as a trigger to other peripherals
 *
 *  2022 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_TIMER_PERIPHERAL_TRIGGER_INTF_HPP
#define CHIMERA_TIMER_PERIPHERAL_TRIGGER_INTF_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <memory>
#include <Chimera/source/drivers/function/function_types.hpp>
#include <Chimera/source/drivers/peripherals/timer/timer_common_types.hpp>
#include <Chimera/source/drivers/peripherals/timer/timer_intf.hpp>
#include <Chimera/source/drivers/peripherals/timer/views/timer_base_intf.hpp>


namespace Chimera::Timer::Trigger
{
  /*---------------------------------------------------------------------------
  Enumerations
  ---------------------------------------------------------------------------*/
  /**
   * @brief Available trigger signals to use as a reference
   *
   * In a Master timer, this indicates what signal controls updating TRGO
   * while in a Slave timer this will indicate what signal acts as an input.
   *
   * @note The mapping of these signals is target device dependent.
   */
  enum class Signal : uint8_t
  {
    TRIG_SIG_1,
    TRIG_SIG_2,
    TRIG_SIG_3,
    TRIG_SIG_4,
    TRIG_SIG_5,
    TRIG_SIG_6,

    TRIG_SIG_NUM_OPTIONS,
    TRIG_SIG_INVALID
  };

  /*---------------------------------------------------------------------------
  Structures
  ---------------------------------------------------------------------------*/
  struct MasterConfig
  {
    Chimera::Timer::CoreConfig coreConfig;  /**< Core timer configuration */
    Chimera::Function::Opaque  isrCallback; /**< Optional callback to call when the trigger fires */
    float                      trigFreq;    /**< Trigger frequency in Hz */

    void clear()
    {
      coreConfig.clear();
      isrCallback = {};
      trigFreq    = -1.0f;
    }
  };

  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/
  /**
   * @brief Configures a Timer for generating a trigger signal on TRGO
   * @warning Consumes the entire timer, erasing any previous configuration.
   *
   * The timer is initialized as a Master device that generates a reference
   * clock or event pulse for other peripherals in the system to consume as
   * a synchronization input.
   */
  class Master
  {
  public:
    Master();
    ~Master();

    /**
     * @brief Initializes the driver
     *
     * @param cfg   The driver configuration settings
     * @return Chimera::Status_t
     */
    Chimera::Status_t init( const MasterConfig &cfg );

    /**
     * @brief Enables the timer
     * @return Chimera::Status_t
     */
    Chimera::Status_t enable();

    /**
     * @brief Disables the timer
     * @return Chimera::Status_t
     */
    Chimera::Status_t disable();

    /**
     * @brief Sets a function to be called on a trigger event
     *
     * @param func  Function to be called
     * @return Chimera::Status_t
     */
    Chimera::Status_t attachISR( Chimera::Function::Opaque func );

    /**
     * @brief Detaches an ISR from the timer, if one is attached
     */
    void detachISR();

    /**
     * @brief Acknowledge the ISR event
     * @note This must be called inside the ISR handler to clear the interrupt
     */
    void ackISR();

  private:
    std::shared_ptr<void*> mTimerImpl; /**< Opaque pointer to the implementer's driver */
  };
}  // namespace Chimera::Timer

#endif  /* !CHIMERA_TIMER_PERIPHERAL_TRIGGER_INTF_HPP */
