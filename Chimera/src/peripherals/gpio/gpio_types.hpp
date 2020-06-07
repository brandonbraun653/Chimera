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
#include <Chimera/src/peripherals/peripheral_types.hpp>

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
    LOW  = false
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

  /**
   *  Alternate function options for all MCU chips. Not all of these will be used
   *  on a given chip, but they will correctly allow the GPIO driver to configure
   *  available pins to their alternate function behavior. See the device datasheet
   *  for more details on available functions and the device driver for details
   *  on how the enumeration here is mapped to a given function.
   */
  enum class Alternate : uint8_t
  {
    COMP1_OUT,
    EVENTOUT,
    I2C1_SCL,
    I2C1_SDA,
    I2C1_SMBA,
    I2C3_SCL,
    IR_OUT,
    JTCKSWCLK,
    JTDI,
    JTMSSWDIO,
    LPTIM1_OUT,
    MCO,
    SPI1_MISO,
    SPI1_MOSI,
    SPI1_NSS,
    SPI1_SCK,
    SPI3_NSS,
    TIM1_BKIN,
    TIM1_BKIN2,
    TIM1_CH1,
    TIM1_CH1N,
    TIM1_CH2,
    TIM1_CH3,
    TIM1_CH4,
    TIM1_ETR,
    TIM2_CH1,
    TIM2_CH2,
    TIM2_CH3,
    TIM2_CH4,
    TIM2_ETR,
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
    USART3_CTS,
    USART3_RTS_DE,
    SAI1_EXTCLK,
    TIM15_CH1N,
    TIM1_CH2N,
    USART3_CK,
    QUADSPI_BK1_IO1,
    TIM1_CH3N,
    LPUART1_RTS_DE,
    QUADSPI_BK1_IO0,
    LPTIM2_IN1,
    JTDO_TRACESWO,
    SPI3_SCK,
    SAI1_SCK_B,
    NJTRST,
    I2C3_SDA,
    SPI3_MISO,
    TSC_G2_IO1,
    SAI1_MCLK_B,
    LPTIM1_IN1,
    SPI3_MOSI,
    TSC_G2_IO2,
    COMP2_OUT,
    SAI1_SD_B,
    TIM16_BKIN,
    LPTIM1_ETR,
    LPTIM1_IN2,
    TSC_G2_IO3,
    SAI1_FS_B,
    TIM16_CH1N,
    TSC_G2_IO4,
    TSC_G3_IO1,
    SWPMI1_SUSPEND,
    LPUART1_TX,
    QUADSPI_BK1_NCS,
    TIM15_CH1,
    SWPMI1_RX,
    LPUART1_RX,
    QUADSPI_CLK,
    SAI1_MCLK_A,
    TIM15_CH2,
    USB_NOE,
    SWPMI1_TX,
    LPTIM2_OUT,
    CAN1_TX,
    USB_DP,
    LPTIM2_ETR,
    CAN1_RX,
    USB_DM,
    TIM1_BKIN2_COMP1,
    USB_CRS_SYNC,
    SAI1_SD_A,
    LPUART1_CTS,
    QUADSPI_BK1_IO3,
    TIM1_BKIN_COMP2,
    TIM16_CH1,
    QUADSPI_BK1_IO2,
    TIM15_BKIN,
    SAI1_FS_A,
    SWPMI1_IO,
    SAI1_SCK_A,
    NONE
  };

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
