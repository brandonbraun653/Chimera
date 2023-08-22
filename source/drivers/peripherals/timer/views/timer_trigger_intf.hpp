/******************************************************************************
 *  File Name:
 *    timer_trigger_intf.hpp
 *
 *  Description:
 *    Timer view for acting as a trigger to other peripherals or reacting to
 *    triggers from other peripherals.
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
    TRIG_SIG_0,
    TRIG_SIG_1,
    TRIG_SIG_2,
    TRIG_SIG_3,
    TRIG_SIG_4,
    TRIG_SIG_5,
    TRIG_SIG_6,

    TRIG_SIG_NUM_OPTIONS,
    TRIG_SIG_INVALID
  };


  enum class SyncAction : uint8_t
  {
    SYNC_RESET,   /**< Trigger input reinitializes counter */
    SYNC_GATE,    /**< Counter clock enabled while trigger is HI, disabled when LO */
    SYNC_TRIGGER, /**< Trigger input starts counter, but doesn't reset it */

    SYNC_NUM_OPTIONS,
    SYNC_INVALID
  };


  /*---------------------------------------------------------------------------
  Structures
  ---------------------------------------------------------------------------*/
  struct MasterConfig
  {
    Chimera::Timer::CoreConfig coreConfig;  /**< Core timer configuration */
    Chimera::Function::Opaque  isrCallback; /**< Optional callback to call when the trigger fires */
    float                      trigFreq;    /**< Output trigger frequency in Hz */

    void clear()
    {
      coreConfig.clear();
      isrCallback = {};
      trigFreq    = -1.0f;
    }
  };


  struct SlaveConfig
  {
    Chimera::Timer::CoreConfig coreConfig;       /**< Core timer configuration */
    Chimera::Function::Opaque  isrCallback;      /**< Optional callback to call when the trigger fires */
    float                      frequency;        /**< Core frequency of the timer before overrun */
    SyncAction                 trigSyncAction;   /**< Action to take on the trigger input */
    Signal                     trigSyncSignal;   /**< Trigger signal to use as an input */


    void clear()
    {
      coreConfig.clear();
      isrCallback      = {};
      frequency        = -1.0f;
      trigSyncAction   = SyncAction::SYNC_INVALID;
      trigSyncSignal   = Signal::TRIG_SIG_INVALID;
    }
  };

  /*---------------------------------------------------------------------------
  Classes
  ---------------------------------------------------------------------------*/

  /**
   * @brief Interface for a timer peripheral that can act as a trigger
   */
  class ITimerTrigger
  {
  public:
    ITimerTrigger();
    ~ITimerTrigger();

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
     * @brief Detaches an ISR from the timer, if one is attached
     */
    void detachISR();

    /**
     * @brief Acknowledge the ISR event
     * @note This must be called inside the ISR handler to clear the interrupt
     */
    void ackISR();

    /**
     * @brief Sets a function to be called on a trigger event
     *
     * @param func  Function to be called
     * @return Chimera::Status_t
     */
    virtual Chimera::Status_t attachISR( Chimera::Function::Opaque func ) = 0;

  protected:
    void *mTimerImpl; /**< Opaque pointer to the implementer's driver */
  };

  /**
   * @brief Configures a Timer for generating an output trigger signal.
   * @warning Consumes the entire timer, erasing any previous configuration.
   *
   * The timer is initialized as a Master device that generates a reference
   * clock or event pulse for other peripherals in the system to consume as
   * a synchronization input.
   */
  class Master : public virtual ITimerTrigger
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

    /*-------------------------------------------------------------------------
    ITimerTrigger Interface
    -------------------------------------------------------------------------*/
    Chimera::Status_t attachISR( Chimera::Function::Opaque func ) final override;
  };


  /**
   * @brief Configures a Timer for reacting to an input trigger signal.
   * @warning Consumes the entire timer, erasing any previous configuration.
   *
   * The timer is initialized as a Slave device that reacts to a reference
   * clock or event pulse from another peripheral in the system.
   *
   * @note May also generate an output trigger signal if desired.
   */
  class Slave : public virtual ITimerTrigger
  {
  public:
    Slave();
    ~Slave();

    /**
     * @brief Initializes the driver.
     *
     * @param cfg   The driver configuration settings
     * @return Chimera::Status_t
     */
    Chimera::Status_t init( const SlaveConfig &cfg );

    /**
     * @brief Gets reload value of the counter, which determines overall period.
     * @return uint32_t
     */
    uint32_t getTickPeriod() const;

    /**
     * @brief Configures the timer to generate an event at a specific offset within its period.
     *
     * This may be used to precisely time when to trigger an interrupt or other peripheral
     * after the timer has been started/reset by the slave input trigger signal.
     *
     * @param tickOffset  Number of ticks to offset the event generation by
     * @return void
     */
    void setEventOffset( const uint32_t tickOffset );

    /*-------------------------------------------------------------------------
    ITimerTrigger Interface
    -------------------------------------------------------------------------*/
    Chimera::Status_t attachISR( Chimera::Function::Opaque func ) final override;
  };
}  // namespace Chimera::Timer::Trigger

#endif /* !CHIMERA_TIMER_PERIPHERAL_TRIGGER_INTF_HPP */
