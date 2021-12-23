/********************************************************************************
 *  File Name:
 *    peripheral_types.hpp
 *
 *  Description:
 *    Types associated with describing peripherals in general.
 *
 *  2019-2021 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_PERIPHERAL_TYPES_HPP
#define CHIMERA_PERIPHERAL_TYPES_HPP

/* STL Includes */
#include <cstdint>
#include <cstddef>

namespace Chimera::Peripheral
{
  /*-------------------------------------------------------------------------------
  Enumerations
  -------------------------------------------------------------------------------*/
  /**
   *  Identifies various kinds of hardware peripherals that could be
   *  supported by Chimera drivers.
   *
   *  @note These enum values are allowed to be used as array indexes so do
   *        not set them to anything other than an ordered set [0,x)
   */
  enum class Type : uint8_t
  {
    PERIPH_ADC = 0, /**< Analog to Digital Peripheral */
    PERIPH_CAN,     /**< Controller Area Network */
    PERIPH_CRC,     /**< Cyclic Redundancy Check */
    PERIPH_CRS,     /**< Clock Recovery System */
    PERIPH_DAC,     /**< Digital to Analog Peripheral */
    PERIPH_DMA,     /**< Direct Memory Access */
    PERIPH_EXTI,    /**< External Interrupt */
    PERIPH_FLASH,   /**< System Flash Controller */
    PERIPH_FMC,     /**< Flexible Memory Controller */
    PERIPH_GPIO,    /**< General Purpose Input Output */
    PERIPH_I2C,     /**< Inter-Integrated Circuit */
    PERIPH_I2S,     /**< Inter-IC Sound */
    PERIPH_IWDG,    /**< Independent Watchdog */
    PERIPH_NVIC,    /**< Nested Vector Interrupt Controller */
    PERIPH_PWR,     /**< Power Controller */
    PERIPH_RCC,     /**< Reset and Clock Controller */
    PERIPH_RTC,     /**< Real Time Clock */
    PERIPH_SDIO,    /**< Secure Digital Input Output */
    PERIPH_SPI,     /**< Serial Peripheral Interface */
    PERIPH_SYSCFG,  /**< System Configuration */
    PERIPH_TIM,     /**< Timer */
    PERIPH_UART,    /**< Universal Asynchronous Receiver Transmitter */
    PERIPH_USART,   /**< Universal Synchronous/Asynchronous Receiver Transmitter */
    PERIPH_VIC,     /**< Vector Interrupt Controller */
    PERIPH_WWDG,    /**< Window Watchdog */
    PERIPH_USB,     /**< Universal Serial Bus */

    NUM_OPTIONS,
    PERIPH_NONE,    /**< Not a peripheral */
    UNKNOWN
  };

  /*-------------------------------------------------------------------------------
  Structures
  -------------------------------------------------------------------------------*/
  /**
   * @brief Describes a connection to a peripheral
   *
   * Mostly this is intended to inform drivers that consume a peripheral
   * which type and instance to consume. For example, an IMU driver may
   * likely operate off of I2C or SPI and may need to know which to select
   * at runtime.
   */
  struct Connection
  {
    Type periphType;  /**< Which peripheral is being described */
    size_t channel;   /**< Hardware channel on the peripheral */
  };
}  // namespace Chimera::Peripheral

#endif /* !CHIMERA_PERIPHERAL_TYPES_HPP */
