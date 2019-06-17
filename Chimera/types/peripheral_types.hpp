/********************************************************************************
 *   File Name:
 *    peripheral_types.hpp
 *
 *   Description:
 *    Types associated with describing peripherals in general.
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_PERIPHERAL_TYPES_HPP
#define CHIMERA_PERIPHERAL_TYPES_HPP

/* C++ Includes */
#include <cstdint>

namespace Chimera::Peripheral
{
  /**
   *  Identifies various kinds of hardware peripherals that could be
   *  supported by Chimera drivers. 
   */
  enum class Type : uint8_t
  {
    ADC,    /**< Analog to Digital Converter */
    CRC,    /**< Cyclic Redundancy Check */
    DAC,    /**< Digital to Analog Converter */
    DMA,    /**< Direct Memory Access */
    FMC,    /**< Flexible Memory Controller */
    GPIO,   /**< General Purpose Input Output */
    I2C,    /**< Inter-Integrated Circuit */
    I2S,    /**< Inter-IC Sound */
    NVIC,   /**< Nested Vector Interrupt Controller */
    PWR,    /**< Power Controller */
    RCC,    /**< Reset and Clock Controller */
    RTC,    /**< Real Time Clock */
    SDIO,   /**< Secure Digital Input Output */
    SPI,    /**< Serial Peripheral Interface */
    TIM,    /**< Timer */
    UART,   /**< Universal Asynchronous Receiver Transmitter */
    USART,  /**< Universal Synchronous/Asynchronous Receiver Transmitter */
    VIC,    /**< Vector Interrupt Controller */
    WDG,    /**< Watchdog */
  };
}

#endif /* !CHIMERA_PERIPHERAL_TYPES_HPP */