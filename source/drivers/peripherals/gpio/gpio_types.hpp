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
  enum class Alternate : uint16_t
  {
    CAN1_RX,
    CAN1_TX,
    COMP1_OUT,
    COMP2_OUT,
    DCMI_D0,
    DCMI_D1,
    DCMI_D10,
    DCMI_D11,
    DCMI_D12,
    DCMI_D13,
    DCMI_D2,
    DCMI_D3,
    DCMI_D4,
    DCMI_D5,
    DCMI_D6,
    DCMI_D7,
    DCMI_D8,
    DCMI_D9,
    DCMI_VSYNC,
    EVENTOUT,
    EXTERNAL_INTERRUPT,
    FMC_A0,
    FMC_A1,
    FMC_A10,
    FMC_A11,
    FMC_A12,
    FMC_A13,
    FMC_A14,
    FMC_A15,
    FMC_A16,
    FMC_A17,
    FMC_A18,
    FMC_A19,
    FMC_A2,
    FMC_A20,
    FMC_A21,
    FMC_A22,
    FMC_A23,
    FMC_A24,
    FMC_A25,
    FMC_A3,
    FMC_A4,
    FMC_A5,
    FMC_A6,
    FMC_A7,
    FMC_A8,
    FMC_A9,
    FMC_CLK,
    FMC_D0,
    FMC_D1,
    FMC_D10,
    FMC_D11,
    FMC_D12,
    FMC_D13,
    FMC_D14,
    FMC_D15,
    FMC_D2,
    FMC_D3,
    FMC_D4,
    FMC_D5,
    FMC_D6,
    FMC_D7,
    FMC_D8,
    FMC_D9,
    FMC_INT,
    FMC_NBL0,
    FMC_NBL1,
    FMC_NE1,
    FMC_NE2,
    FMC_NE3,
    FMC_NE4,
    FMC_NOE,
    FMC_NWAIT,
    FMC_NWE,
    FMC_SDCKE0,
    FMC_SDCLK,
    FMC_SDNCAS,
    FMC_SDNE0,
    FMC_SDNRAS,
    FMC_SDNWE,
    FMPI2C1_SCL,
    FMPI2C1_SDA,
    FMPI2C1_SMBA,
    I2C1_SCL,
    I2C1_SDA,
    I2C1_SMBA,
    I2C2_SCL,
    I2C2_SDA,
    I2C2_SMBA,
    I2C3_SCL,
    I2C3_SDA,
    I2S1_MCK,
    I2S2_MCK,
    I2S3_MCK,
    I2S_CKIN,
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
    MCO2,
    NJTRST,
    OTG_HS_ULPI_DIR,
    OTG_HS_ULPI_NXT,
    OTG_HS_ULPI_STP,
    PMPI2C1_SCL,
    QUADSPI_BK1_IO0,
    QUADSPI_BK1_IO1,
    QUADSPI_BK1_IO2,
    QUADSPI_BK1_IO3,
    QUADSPI_BK1_NCS,
    QUADSPI_BK2_IO0,
    QUADSPI_BK2_IO1,
    QUADSPI_BK2_IO2,
    QUADSPI_BK2_IO3,
    QUADSPI_BK2_NCS,
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
    SAI2_FS_A,
    SAI2_FS_B,
    SAI2_MCLK_A,
    SAI2_MCLK_B,
    SAI2_SCK_A,
    SAI2_SCK_B,
    SAI2_SD_A,
    SAI2_SD_B,
    SDIO_CK,
    SDIO_CMD,
    SDIO_D0,
    SDIO_D1,
    SDIO_D2,
    SDIO_D3,
    SDIO_D6,
    SDIO_D7,
    SPDIFRX_IN0,
    SPDIFRX_IN1,
    SPDIFRX_IN2,
    SPDIFRX_IN3,
    SPDIF_RX0,
    SPDIF_RX1,
    SPDIF_RX2,
    SPDIF_RX3,
    SPI1_MISO,
    SPI1_MOSI,
    SPI1_NSS,
    SPI1_SCK,
    SPI2_MISO,
    SPI2_MOSI,
    SPI2_NSS,
    SPI2_SCK,
    SPI3_MISO,
    SPI3_MOSI,
    SPI3_NSS,
    SPI3_SCK,
    SPI4_MISO,
    SPI4_MOSI,
    SPI4_NSS,
    SPI4_SCK,
    SWPMI1_IO,
    SWPMI1_RX,
    SWPMI1_SUSPEND,
    SWPMI1_TX,
    TIM10_CH1,
    TIM11_CH1,
    TIM13_CH1,
    TIM14_CH1,
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
    TIM3_CH1,
    TIM3_CH2,
    TIM3_CH3,
    TIM3_CH4,
    TIM3_ETR,
    TIM4_CH1,
    TIM4_CH2,
    TIM4_CH3,
    TIM4_CH4,
    TIM4_ETR,
    TIM5_CH1,
    TIM8_CH1,
    TIM8_CH2,
    TIM8_CH3,
    TIM8_CH4,
    TIM8_ETR,
    TIM9_CH1,
    TIM9_CH2,
    TRACE_CLK,
    TRACE_D0,
    TRACE_D1,
    TRACE_D2,
    TRACE_D3,
    TSC_G2_IO1,
    TSC_G2_IO2,
    TSC_G2_IO3,
    TSC_G2_IO4,
    TSC_G3_IO1,
    UART4_RX,
    UART4_TX,
    UART5_CTS,
    UART5_RTS,
    UART5_RX,
    UART5_TX,
    USART1_CK,
    USART1_CTS,
    USART1_RTS_DE,
    USART1_RX,
    USART1_TX,
    USART2_CK,
    USART2_CTS,
    USART2_RTS,
    USART2_RTS_DE,
    USART2_RX,
    USART2_TX,
    USART3_CK,
    USART3_CTS,
    USART3_RTS,
    USART3_RTS_DE,
    USART3_RX,
    USART3_TX,
    USART6_CK,
    USART6_CTS,
    USART6_RTS,
    USART6_RX,
    USART6_TX,
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
      Driver_rPtr ( *getDriver )( const Port port, const Pin pin );
    };
  }  // namespace Backend
}  // namespace Chimera::GPIO

#endif /* !CHIMERA_GPIO_TYPES_HPP */
