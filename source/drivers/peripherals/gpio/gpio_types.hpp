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
#include <Chimera/source/drivers/peripherals/peripheral_types.hpp>

namespace Chimera::GPIO
{
  /*-------------------------------------------------------------------------------
  Forward Declarations
  -------------------------------------------------------------------------------*/
  class Driver;

  /*-------------------------------------------------------------------------------
  Aliases
  -------------------------------------------------------------------------------*/
  using Pin = uint8_t;
  using Driver_rPtr = Driver *;
  using Driver_sPtr = std::shared_ptr<Driver>;

  /*-------------------------------------------------------------------------------
  Enumerations
  -------------------------------------------------------------------------------*/
  enum class Drive : uint8_t
  {
    INPUT,
    OUTPUT_PUSH_PULL,
    OUTPUT_OPEN_DRAIN,
    ALTERNATE_PUSH_PULL,
    ALTERNATE_OPEN_DRAIN,
    ANALOG,
    HIZ,

    NUM_OPTIONS,
    UNKNOWN_DRIVE
  };

  enum class State : bool
  {
    HIGH = true,
    LOW  = false
  };

  enum class Pull : uint8_t
  {
    NO_PULL,
    PULL_UP,
    PULL_DN,

    NUM_OPTIONS,
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

    NUM_OPTIONS,
    UNKNOWN_PORT
  };

  /**
   *  Alternate function options for all MCU chips. Not all of these will be used
   *  on a given chip, but they will correctly allow the GPIO driver to configure
   *  available pins to their alternate function behavior. See the device datasheet
   *  for more details on available functions and the device driver for details
   *  on how the enumeration here is mapped to a given function.
   */
  enum class Alternate : uint8_t
  {
    CAN1_RX,
    CAN1_TX,
    COMP1_OUT,
    COMP2_OUT,
    EVENTOUT,
    EXTERNAL_INTERRUPT,
    I2C1_SCL,
    I2C1_SDA,
    I2C1_SMBA,
    I2C3_SCL,
    I2C3_SDA,
    IR_OUT,
    JTCKSWCLK,
    JTDI,
    JTDO_TRACESWO,
    JTMSSWDIO,
    LPTIM1_ETR,
    LPTIM1_IN1,
    LPTIM1_IN2,
    LPTIM1_OUT,
    LPTIM2_ETR,
    LPTIM2_IN1,
    LPTIM2_OUT,
    LPUART1_CTS,
    LPUART1_RTS_DE,
    LPUART1_RX,
    LPUART1_TX,
    MCO,
    NJTRST,
    QUADSPI_BK1_IO0,
    QUADSPI_BK1_IO1,
    QUADSPI_BK1_IO2,
    QUADSPI_BK1_IO3,
    QUADSPI_BK1_NCS,
    QUADSPI_CLK,
    SAI1_EXTCLK,
    SAI1_FS_A,
    SAI1_FS_B,
    SAI1_MCLK_A,
    SAI1_MCLK_B,
    SAI1_SCK_A,
    SAI1_SCK_B,
    SAI1_SD_A,
    SAI1_SD_B,
    SPI1_MISO,
    SPI1_MOSI,
    SPI1_NSS,
    SPI1_SCK,
    SPI3_MISO,
    SPI3_MOSI,
    SPI3_NSS,
    SPI3_SCK,
    SWPMI1_IO,
    SWPMI1_RX,
    SWPMI1_SUSPEND,
    SWPMI1_TX,
    TIM15_BKIN,
    TIM15_CH1,
    TIM15_CH1N,
    TIM15_CH2,
    TIM16_BKIN,
    TIM16_CH1,
    TIM16_CH1N,
    TIM1_BKIN,
    TIM1_BKIN2,
    TIM1_BKIN2_COMP1,
    TIM1_BKIN_COMP2,
    TIM1_CH1,
    TIM1_CH1N,
    TIM1_CH2,
    TIM1_CH2N,
    TIM1_CH3,
    TIM1_CH3N,
    TIM1_CH4,
    TIM1_ETR,
    TIM2_CH1,
    TIM2_CH2,
    TIM2_CH3,
    TIM2_CH4,
    TIM2_ETR,
    TSC_G2_IO1,
    TSC_G2_IO2,
    TSC_G2_IO3,
    TSC_G2_IO4,
    TSC_G3_IO1,
    USART1_CK,
    USART1_CTS,
    USART1_RTS_DE,
    USART1_RX,
    USART1_TX,
    USART2_CK,
    USART2_CTS,
    USART2_RTS_DE,
    USART2_RX,
    USART2_TX,
    USART3_CK,
    USART3_CTS,
    USART3_RTS_DE,
    USB_CRS_SYNC,
    USB_DM,
    USB_DP,
    USB_NOE,

    NUM_OPTIONS,
    NONE
  };

  /*-------------------------------------------------------------------------------
  Structures
  -------------------------------------------------------------------------------*/
  struct PinInit
  {
    Alternate alternate; /**< What peripheral type the pin should be configured as */
    Drive drive;         /**< Pin IO drive type */
    Pin pin;             /**< Pin number on the given port */
    Port port;           /**< Pin IO port */
    Pull pull;           /**< Pull-up/down configuration */
    State state;         /**< Default logical pin state on init */
    bool threaded;       /**< Optionally enable thread safe access */
    bool validity;       /**< Indicates if the config should be initialized */

    void clear()
    {
      pull      = Pull::UNKNOWN_PULL;
      port      = Port::UNKNOWN_PORT;
      drive     = Drive::INPUT;
      state     = State::LOW;
      pin       = 0u;
      alternate = Alternate::NONE;
      validity  = false;
    }
  };

  namespace Backend
  {
    struct DriverConfig
    {
      bool isSupported; /**< A simple flag to let Chimera know if the driver is supported */

      /**
       *  Function pointer that initializes the backend driver's
       *  memory. Should really only call once for initial set up.
       */
      Chimera::Status_t ( *initialize )();

      /**
       *  Resets the backend driver hardware to default configuration
       *  settings, but does not wipe out any memory.
       */
      Chimera::Status_t ( *reset )();

      /**
       *  Factory function that creates a shared_ptr instance of the backend
       *  driver, as long as it conforms to the expected interface.
       */
      Driver_sPtr ( *getDriver )( const Port port, const Pin pin );
    };
  }  // namespace Backend
}  // namespace Chimera::GPIO

#endif /* !CHIMERA_GPIO_TYPES_HPP */
