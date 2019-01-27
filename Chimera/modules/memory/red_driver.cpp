/********************************************************************************
*   File Name:
*       red_driver.cpp
*
*   Description:
*       Implements the C++ API wrapper around the Reliance-Edge FS
*
*   2019 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/


/* Driver Includes */
#include <Chimera/modules/memory/red_driver.hpp>
#include <Chimera/modules/memory/red_stub.hpp>


#ifdef __cplusplus
extern "C" {
#endif

REDSTATUS DiskOpen(
    uint8_t         bVolNum,
    BDEVOPENMODE    mode)
{
    REDSTATUS       ret;

    /*  Avoid warnings about unused function parameters.
    */
    (void)bVolNum;
    (void)mode;

    /*  Insert code here to open/initialize the block device.
    */
    REDERROR();
    ret = -RED_ENOSYS;

    return ret;
}


/** @brief Uninitialize a disk.

    @param bVolNum  The volume number of the volume whose block device is being
                    uninitialized.

    @return A negated ::REDSTATUS code indicating the operation result.

    @retval 0   Operation was successful.
*/
REDSTATUS DiskClose(
    uint8_t     bVolNum)
{
    REDSTATUS   ret;

    /*  Avoid warnings about unused function parameters.
    */
    (void)bVolNum;

    /*  Insert code here to close/deinitialize the block device.
    */
    REDERROR();
    ret = -RED_ENOSYS;

    return ret;
}


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
REDSTATUS DiskRead(
    uint8_t     bVolNum,
    uint64_t    ullSectorStart,
    uint32_t    ulSectorCount,
    void       *pBuffer)
{
    REDSTATUS   ret;

    /*  Avoid warnings about unused function parameters.
    */
    (void)bVolNum;
    (void)ullSectorStart;
    (void)ulSectorCount;
    (void)pBuffer;

    /*  Insert code here to read sectors from the block device.
    */
    REDERROR();
    ret = -RED_ENOSYS;

    return ret;
}


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
REDSTATUS DiskWrite(
    uint8_t     bVolNum,
    uint64_t    ullSectorStart,
    uint32_t    ulSectorCount,
    const void *pBuffer)
{
    REDSTATUS   ret;

    /*  Avoid warnings about unused function parameters.
    */
    (void)bVolNum;
    (void)ullSectorStart;
    (void)ulSectorCount;
    (void)pBuffer;

    /*  Insert code here to write sectors to the block device.
    */
    REDERROR();
    ret = -RED_ENOSYS;

    return ret;
}


/** @brief Flush any caches beneath the file system.

    @param bVolNum  The volume number of the volume whose block device is being
                    flushed.

    @return A negated ::REDSTATUS code indicating the operation result.

    @retval 0           Operation was successful.
    @retval -RED_EIO    A disk I/O error occurred.
*/
REDSTATUS DiskFlush(
    uint8_t     bVolNum)
{
    REDSTATUS   ret;

    /*  Avoid warnings about unused function parameters.
    */
    (void)bVolNum;

    /*  Insert code here to flush the block device.  If writing to the block
        device is inherently synchronous (no hardware or software cache), then
        this can do nothing and return success.
    */
    REDERROR();
    ret = -RED_ENOSYS;

    return ret;
}
#endif /* !REDCONF_READ_ONLY == 0 */

#ifdef __cplusplus
}
#endif
