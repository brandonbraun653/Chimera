/********************************************************************************
 *  File Name:
 *    peripheral_types.hpp
 *
 *  Description:
 *    Types associated with describing peripherals in general.
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_PERIPHERAL_TYPES_HPP
#define CHIMERA_PERIPHERAL_TYPES_HPP

/* STL Includes */
#include <cstdint>

namespace Chimera::Peripheral
{
  /**
   *  Identifies various kinds of hardware peripherals that could be
   *  supported by Chimera drivers.
   *
   *  @note These enum values are allowed to be used as array indexes so do
   *        not set them to anything other than an ordered set [0,x)
   */
  enum class Type : uint8_t
  {
    PERIPH_ADC = 0, /**< Analog to Digital Converter */
    PERIPH_CRC,     /**< Cyclic Redundancy Check */
    PERIPH_DAC,     /**< Digital to Analog Converter */
    PERIPH_DMA,     /**< Direct Memory Access */
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
    PERIPH_TIM,     /**< Timer */
    PERIPH_UART,    /**< Universal Asynchronous Receiver Transmitter */
    PERIPH_USART,   /**< Universal Synchronous/Asynchronous Receiver Transmitter */
    PERIPH_VIC,     /**< Vector Interrupt Controller */
    PERIPH_WWDG,    /**< Window Watchdog */

    PERIPH_NONE, /**< Not a peripheral */
    NUM_SUPPORTED_TYPES
  };
}  // namespace Chimera::Peripheral

#endif /* !CHIMERA_PERIPHERAL_TYPES_HPP */
