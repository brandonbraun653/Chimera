/********************************************************************************
 *   File Name:
 *       red_driver.cpp
 *
 *   Description:
 *       Implements the C++ API wrapper around the Reliance-Edge FS
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

 /* Reliance Edge FS Includes */
 #include "redposix.h"

/* Chimera Includes */
#include <Chimera/modules/memory/red_driver.hpp>
#include <Chimera/modules/memory/red_stub.hpp>

/* C++ Includes (keep below Chimera) */
#include <array>

using namespace Chimera::Modules::Memory;

static std::array<BlockDevice_sPtr, Red::MAX_MEMORY_DEVICES> memoryDevices;
static std::array<uint8_t, Red::MAX_MEMORY_DEVICES> deviceToVolNum;

namespace Red
{
    REDSTATUS RedFS::init(BlockDevice_sPtr &device)
    {
        // Find the next empty slot...record the index

        return red_init();
    }
}

/**
*   Looks up the device index that can be used to retrieve a memory device object
*
*   @param[in]  volNum      The volume number associated with a device
*   @return Index of the device tied to the volume number
*/
static uint8_t getDeviceIndex(const uint8_t volNum)
{
    uint8_t foundDevice = Red::MAX_MEMORY_DEVICES;

    for (uint8_t device = 0; device < deviceToVolNum.size(); device++)
    {
        if (deviceToVolNum[device] == volNum)
        {
            foundDevice = device;
            break;
        }
    }

    return foundDevice;
}

#ifdef __cplusplus
extern "C" {
#endif

REDSTATUS DiskOpen(uint8_t bVolNum, BDEVOPENMODE mode)
{
    REDSTATUS ret = -RED_EINVAL;

    uint8_t devIdx = getDeviceIndex(bVolNum);

    if (devIdx < Red::MAX_MEMORY_DEVICES)
    {
        const BlockDevice_sPtr &device = memoryDevices[devIdx];

        if (device)
        {
            BlockStatus errCode = device->DiskOpen(bVolNum, static_cast<BlockMode>(mode));
            ret = static_cast<REDSTATUS>(errCode);
        }
    }

    return ret;
}

REDSTATUS DiskClose(uint8_t bVolNum)
{
    REDSTATUS ret = -RED_EINVAL;

    uint8_t devIdx = getDeviceIndex(bVolNum);

    if (devIdx < Red::MAX_MEMORY_DEVICES)
    {
        const BlockDevice_sPtr &device = memoryDevices[devIdx];

        if (device)
        {
            BlockStatus errCode = device->DiskClose(bVolNum);
            ret = static_cast<REDSTATUS>(errCode);
        }
    }

    return ret;
}

REDSTATUS DiskRead(uint8_t bVolNum, uint64_t ullSectorStart, uint32_t ulSectorCount, void *pBuffer)
{
    REDSTATUS ret = -RED_EINVAL;

    uint8_t devIdx = getDeviceIndex(bVolNum);

    if (devIdx < Red::MAX_MEMORY_DEVICES)
    {
        const BlockDevice_sPtr &device = memoryDevices[devIdx];

        if (device)
        {
            BlockStatus errCode = device->DiskRead(bVolNum, ullSectorStart, ulSectorCount, pBuffer);
            ret = static_cast<REDSTATUS>(errCode);
        }
    }

    return ret;
}

#if REDCONF_READ_ONLY == 0
REDSTATUS DiskWrite(uint8_t bVolNum, uint64_t ullSectorStart, uint32_t ulSectorCount, const void *pBuffer)
{
    REDSTATUS ret = -RED_EINVAL;

    uint8_t devIdx = getDeviceIndex(bVolNum);

    if (devIdx < Red::MAX_MEMORY_DEVICES)
    {
        const BlockDevice_sPtr &device = memoryDevices[devIdx];

        if (device)
        {
            BlockStatus errCode = device->DiskWrite(bVolNum, ullSectorStart, ulSectorCount, pBuffer);
            ret = static_cast<REDSTATUS>(errCode);
        }
    }

    return ret;
}

REDSTATUS DiskFlush(uint8_t bVolNum)
{
    REDSTATUS ret = -RED_EINVAL;

    uint8_t devIdx = getDeviceIndex(bVolNum);

    if (devIdx < Red::MAX_MEMORY_DEVICES)
    {
        const BlockDevice_sPtr &device = memoryDevices[devIdx];

        if (device)
        {
            BlockStatus errCode = device->DiskFlush(bVolNum);
            ret = static_cast<REDSTATUS>(errCode);
        }
    }

    return ret;
}
#endif /* !REDCONF_READ_ONLY == 0 */

#ifdef __cplusplus
}
#endif
