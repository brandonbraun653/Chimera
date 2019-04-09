/********************************************************************************
 *   File Name:
 *    chimera_crc.cpp
 *
 *   Description:
 *    Implements the software CRC algorithm
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Boost Includes */
#include <boost/crc.hpp>

/* Chimera Includes */
#include <Chimera/crc.hpp>

namespace Chimera
{
  namespace AlgCRC
  {
    SW::SW()
    {
      poly = 0u;
      bits = 0u;
      previous = 0u;
    }

    Chimera::Status_t SW::init( const uint32_t polynomial, const uint8_t crcWidth )
    {
      poly = polynomial;
      bits = crcWidth;
      return Chimera::CommonStatusCodes::OK;
    }

    uint32_t SW::accumulate( const uint32_t *const buffer, const uint32_t length )
    {
      return 0u;
    }

    uint32_t SW::calculate( const uint32_t *const buffer, const uint32_t length )
    {
      return 0u;
    }

    uint32_t SW::getPolynomial()
    {
      return poly;
    }
  }
}