/******************************************************************************
 *  File Name:
 *    interrupt_types.hpp
 *
 *  Description:
 *    Interrupt type definitions
 *
 *  2021-2023 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_INTERRUPT_TYPES_HPP
#define CHIMERA_INTERRUPT_TYPES_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <cstdint>
#include <limits>
#include <Chimera/common>
#include <Chimera/peripheral>
#include <etl/delegate.h>

namespace Chimera::Interrupt
{
  /*---------------------------------------------------------------------------
  Forward Declarations
  ---------------------------------------------------------------------------*/
  struct SignalCallback;

  /*---------------------------------------------------------------------------
  Aliases
  ---------------------------------------------------------------------------*/

  using Signal_t = int16_t;

  /**
   *  A fast callback will execute directly inside the ISR. This should
   *  be kept extremely short and **not** access any kind of system
   *  resource, except those tagged with the 'fromISR' keywords.
   *
   *  @return void
   */
  using ISRCallback = etl::delegate<void( void )>;


  /**
   *  A thread aware callback will execute inside a high priority thread
   *  registered with the peripheral interupt. This allows far more complex
   *  and time consuming tasks to execute, but at the cost of latency.
   *
   *  @param event     Pointer to this structure
   *  @return void
   */
  using UserCallback = etl::delegate<void( const SignalCallback *const event )>;

  /*---------------------------------------------------------------------------
  Constants
  ---------------------------------------------------------------------------*/

  /**
   * @brief Indicates an invalid interrupt signal
   */
  static constexpr Signal_t SIGNAL_INVALID = std::numeric_limits<Signal_t>::min();

  /*---------------------------------------------------------------------------
  Structures
  ---------------------------------------------------------------------------*/

  /**
   * @brief Provides a way to configure the priority of peripheral ISRs.
   *
   * Most of the HW drivers in Chimera use a combination of ISRs and threads to
   * handle the various events that can occur. The ISRs are used to handle the
   * low latency events, while the threads are used to handle the more complex
   * events that require more time to process. This structure provides a way
   * to configure the priority of the ISR and the thread.
   */
  struct ISRCfg
  {
    uint32_t priority; /**< Core preemption priority of the ISR */
    uint32_t sub;      /**< Secondary priority for ISRs at same level */
    uint32_t thread;   /**< Priority of dedicated HW thread */
  };

  /**
   * @brief Register a callback to be invoked when a particular interrupt fires
   */
  struct SignalCallback
  {
    void        *data;         /**< Possible event data from the ISR */
    uint16_t     size;         /**< Number of bytes contained in the 'data' pointer */
    Signal_t     signal;       /**< Which signal is associated with this data */
    ISRCallback  isrCallback;  /**< Optional callback to run inside the ISR */
    UserCallback userCallback; /**< Optional callback to run inside user thread */
  };

  namespace Backend
  {
    struct DriverConfig
    {
      bool isSupported; /**< Whether or not the driver is supported */

      /**
       * @brief Initialize the driver
       * @return Chimera::Status_t
       */
      Chimera::Status_t ( *initialize )( void );

      /**
       * @brief Reset the driver
       * @return Chimer::Status_t
       */
      Chimera::Status_t ( *reset )( void );

      /**
       *  @brief Registers a set of handler functions for a peripheral ISR signal.
       *
       *  @param type      Which peripheral to register this against
       *  @param signal    The ISR event being listened to
       *  @param callback  The callback structure being registered
       *  @return Chimera::Status_t
       */
      Chimera::Status_t ( *registerISRHandler )( const Peripheral::Type type, const Signal_t signal,
                                                 const SignalCallback &callback );

      /**
       * @brief Disables the given ISR signal
       *
       * @param signal      The ISR signal to disable
       * @return Chimera::Status_t
       */
      Chimera::Status_t ( *disableISR )( const Signal_t signal );

      /**
       * @brief Enables the given ISR signal
       *
       * @param signal      The ISR signal to enable
       * @return Chimera::Status_t
       */
      Chimera::Status_t ( *enableISR )( const Signal_t signal );
    };
  }  // namespace Backend

  /*---------------------------------------------------------------------------
  Aliases
  ---------------------------------------------------------------------------*/

  /**
   *  Allows a peripheral driver to hold a number of ISR callback
   *  structures, indexed by a driver defined lookup value.
   */
  template<const size_t SIZE>
  using CallbackArray = std::array<SignalCallback, SIZE>;

}  // namespace Chimera::Interrupt

#endif /* !CHIMERA_INTERRUPT_TYPES_HPP */
