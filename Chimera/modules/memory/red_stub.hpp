/********************************************************************************
*   File Name:
*       red_stub.hpp
*
*   Description:
*       Provides the low level stub function definitions used in the Reliance-Edge
*       file system. These are implemented inside of red_driver.cpp.
*
*   2019 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/

#ifndef CHIMERA_RED_STUB_HPP
#define CHIMERA_RED_STUB_HPP

#ifdef __cplusplus
extern "C" {
#endif

/* C Includes */
#include <stdlib.h>
#include <stdint.h>


/* Driver Includes */
#include "rederrno.h"
#include "redosserv.h"
#include "redutils.h"

REDSTATUS DiskOpen(uint8_t bVolNum, BDEVOPENMODE mode);

/** @brief Uninitialize a disk.

    @param bVolNum  The volume number of the volume whose block device is being
                    uninitialized.

    @return A negated ::REDSTATUS code indicating the operation result.

    @retval 0   Operation was successful.
*/
REDSTATUS DiskClose(uint8_t bVolNum);

/** @brief Read sectors from a disk.

    @param bVolNum          The volume number of the volume whose block device
                            is being read from.
    @param ullSectorStart   The starting sector number.
    @param ulSectorCount    The number of sectors to read.
    @param pBuffer          The buffer into which to read the sector data.

    @return A negated ::REDSTATUS code indicating the operation result.

    @retval 0           Operation was successful.
    @retval -RED_EIO    A disk I/O error occurred.
*/
REDSTATUS DiskRead(uint8_t bVolNum, uint64_t ullSectorStart, uint32_t ulSectorCount, void *pBuffer);

#if REDCONF_READ_ONLY == 0

/** @brief Write sectors to a disk.

    @param bVolNum          The volume number of the volume whose block device
                            is being written to.
    @param ullSectorStart   The starting sector number.
    @param ulSectorCount    The number of sectors to write.
    @param pBuffer          The buffer from which to write the sector data.

    @return A negated ::REDSTATUS code indicating the operation result.

    @retval 0           Operation was successful.
    @retval -RED_EIO    A disk I/O error occurred.
*/
REDSTATUS DiskWrite(uint8_t bVolNum, uint64_t ullSectorStart, uint32_t ulSectorCount, const void *pBuffer);

/** @brief Flush any caches beneath the file system.

    @param bVolNum  The volume number of the volume whose block device is being
                    flushed.

    @return A negated ::REDSTATUS code indicating the operation result.

    @retval 0           Operation was successful.
    @retval -RED_EIO    A disk I/O error occurred.
*/
REDSTATUS DiskFlush(uint8_t bVolNum);
#endif /* !REDCONF_READ_ONLY == 0 */

#ifdef __cplusplus
}
#endif

#endif /* !CHIMERA_RED_STUB_HPP */
