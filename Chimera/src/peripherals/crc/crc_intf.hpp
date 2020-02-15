/********************************************************************************
 *  File Name:
 *    crc_intf.hpp
 *
 *  Description:
 *    Models the Chimera CRC interface
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_CRC_INTERFACE_HPP
#define CHIMERA_CRC_INTERFACE_HPP

/* STL Includes */
#include <cstdint>

/* Chimera Includes */
#include <Chimera/common>

namespace Chimera::HWCRC
{
  class HWInterface
  {
  public:
    virtual ~HWInterface() = default;

    /**
     *  Initializes the CRC unit. Some devices may not be configurable, so the code
     *  using this module should check that the expected CRC polynomial was set using
     *  getPolynomial().
     *
     *  @see getPolynomial
     *
     *  @param[in]  polynomial      The desired polynomial to calculate the CRC with
     *  @param[in]  crcWidth        The desired CRC width
     *  @return Chimera::Status_t
     *
     *  |  Return Value |          Explanation          |
     *  |:-------------:|:-----------------------------:|
     *  |            OK | The initialization succeeded  |
     *  |          FAIL | The initialization failed     |
     *  | NOT_SUPPORTED | Hardware CRC is not supported |
     */
    virtual Chimera::Status_t init( const uint32_t polynomial, const uint8_t crcWidth ) = 0;

    /**
     *  Calculates a new CRC using the results of the last CRC calculation as a starting point
     *
     *  @param[in]  buffer          Data to calculate the CRC on
     *  @param[in]  length          The number of bytes contained in the buffer
     *  @return uint32_t
     *
     *  | Return Value |     Explanation    |
     *  |:------------:|:------------------:|
     *  |          Any | The calculated CRC |
     */
    virtual uint32_t accumulate( const uint32_t *const buffer, const uint32_t length ) = 0;

    /**
     *  Calculates a new CRC from scratch, tossing out the results of any previous calculation
     *
     *  @param[in]  buffer          Data to calculate the CRC on
     *  @param[in]  length          The number of bytes contained in the buffer
     *  @return uint32_t
     *
     *  | Return Value |     Explanation    |
     *  |:------------:|:------------------:|
     *  |          Any | The calculated CRC |
     */
    virtual uint32_t calculate( const uint32_t *const buffer, const uint32_t length ) = 0;

    /**
     *  Gets the current polynomial used to calculate the CRC
     *
     *  @return uint32_t
     *
     *  |   Return Value  |      Explanation      |
     *  |:---------------:|:---------------------:|
     *  |               0 | An invalid polynomial |
     *  | Any other value | A valid polynomial    |
     */
    virtual uint32_t getPolynomial() = 0;
  };

  class ICRC : virtual public HWInterface
  {
  public:
    virtual ~ICRC() = default;
  };

}  // namespace Chimera::AlgCRC

#endif /* !CHIMERA_CRC_INTERFACE_HPP */