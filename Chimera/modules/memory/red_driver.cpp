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
#include <Chimera/threading.hpp>

/* C++ Includes (keep below Chimera) */
#include <array>
#include <cassert>

namespace CMem = Chimera::Modules::Memory;

/*------------------------------------------------
Static resources used to cache memory devices currently
registered with the backend driver.
------------------------------------------------*/
static std::array<CMem::BlockDevice_sPtr, Red::MAX_MEMORY_DEVICES> memoryDevices;
static_assert(memoryDevices.size() == Red::MAX_MEMORY_DEVICES, "Invalid array size");

static SemaphoreHandle_t memoryDevices_mtx = xSemaphoreCreateMutex();
static TickType_t memoryDevices_timeout = pdMS_TO_TICKS(5);

/*------------------------------------------------
Static resources for mapping which device is attached to a volume number
------------------------------------------------*/
static std::array<uint8_t, Red::MAX_MEMORY_DEVICES> deviceToVolNum;
static_assert(deviceToVolNum.size() == Red::MAX_MEMORY_DEVICES, "Invalid array size");

static SemaphoreHandle_t deviceToVolNum_mtx = xSemaphoreCreateMutex();
static TickType_t deviceToVolNum_timeout = pdMS_TO_TICKS(5);

namespace Red
{
    REDSTATUS RedFSPosix::init(CMem::BlockDevice_sPtr &device, std::string volumeName)
    {
        bool registrationOk = false;
        REDSTATUS errorCode = -1;

        /*------------------------------------------------
        Protect against concurrent access from multiple threads since
        Reliance Edge does not provide any.
        ------------------------------------------------*/
        if (xSemaphoreTake(memoryDevices_mtx, memoryDevices_timeout) == pdPASS)
        {
            /*------------------------------------------------
            Do we have space to keep track of a new block memory device?
            ------------------------------------------------*/
            for (uint8_t i = 0; i < memoryDevices.size(); i++)
            {
                if (memoryDevices[i] == nullptr)
                {
                    /*------------------------------------------------
                    Intentionally create a new shared_ptr object to increment the reference
                    count. We don't want this object going away suddenly!
                    ------------------------------------------------*/
                    memoryDevices[i] = device;
                    registeredVolume = volumeName;

                    /*------------------------------------------------
                    Keep track of which index the device was initialized at as well as
                    the location in memory of the actual object. This will be used later
                    in uninit() to make sure we are deleting the correct object.
                    ------------------------------------------------*/
                    registeredDeviceIndex = i;
                    registeredDeviceAddress = reinterpret_cast<uint32_t>(device.get());
                    registrationOk = true;
                    break;
                }
                else if (memoryDevices[i] == device)
                {
                    registrationOk = true;
                    break;
                }
            }

            /*------------------------------------------------
            Keep in mind that red_init() is NOT thread safe.
            ------------------------------------------------*/
            if (registrationOk)
            {
                errorCode = red_init();
            }
            else
            {
                blockDeviceErr = CMem::BlockStatus::BLOCK_DEV_ENOINIT;
            }

            xSemaphoreGive(memoryDevices_mtx);
        }
        else
        {
            blockDeviceErr = CMem::BlockStatus::BLOCK_DEV_LOCKED;
        }

        return errorCode;
    }

    REDSTATUS RedFSPosix::deInitFileSystem()
    {
        return red_uninit();
    }

    bool RedFSPosix::deInitBlockDevice()
    {
        assert(registeredDeviceIndex < memoryDevices.size());

        bool removalOk = false;

        /*------------------------------------------------
        Try and remove the device directly
        ------------------------------------------------*/
        if (registeredDeviceIndex < memoryDevices.size())
        {
            uint32_t candidateAddress = reinterpret_cast<uint32_t>(memoryDevices[registeredDeviceIndex].get());

            if (candidateAddress == registeredDeviceAddress)
            {
                memoryDevices[registeredDeviceIndex] = nullptr;
                registeredDeviceIndex = memoryDevices.size();
                registeredDeviceAddress = 0u;
                removalOk = true;
            }
        }
        /*------------------------------------------------
        Otherwise, we are going to have to search through all the registered devices.
        ------------------------------------------------*/
        else if(!removalOk)
        {
            uint32_t candidateAddress = 0u;

            for (uint8_t i = 0; i < memoryDevices.size(); i++)
            {
                candidateAddress = reinterpret_cast<uint32_t>(memoryDevices[i].get());

                if (candidateAddress == registeredDeviceAddress)
                {
                    memoryDevices[i] = nullptr;
                    registeredDeviceIndex = memoryDevices.size();
                    registeredDeviceAddress = 0u;
                    removalOk = true;
                    break;
                }
            }
        }

        if (!removalOk)
        {
            blockDeviceErr = CMem::BlockStatus::BLOCK_DEV_ENODEINIT;
        }

        return removalOk;
    }

    REDSTATUS RedFSPosix::mount()
    {
      return red_mount( registeredVolume.c_str() );
    }

    REDSTATUS RedFSPosix::unmount()
    {
      return red_umount( registeredVolume.c_str() );
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

    if (xSemaphoreTake(deviceToVolNum_mtx, deviceToVolNum_timeout) == pdPASS)
    {
        for (uint8_t device = 0; device < deviceToVolNum.size(); device++)
        {
            if (deviceToVolNum[device] == volNum)
            {
                foundDevice = device;
                break;
            }
        }

        xSemaphoreGive(deviceToVolNum_mtx);
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
        const CMem::BlockDevice_sPtr &device = memoryDevices[devIdx];

        if (device)
        {
            CMem::BlockStatus errCode = device->DiskOpen(bVolNum, static_cast<CMem::BlockMode>(mode));
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
        const CMem::BlockDevice_sPtr &device = memoryDevices[devIdx];

        if (device)
        {
            CMem::BlockStatus errCode = device->DiskClose(bVolNum);
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
        const CMem::BlockDevice_sPtr &device = memoryDevices[devIdx];

        if (device)
        {
            CMem::BlockStatus errCode = device->DiskRead(bVolNum, ullSectorStart, ulSectorCount, pBuffer);
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
        const CMem::BlockDevice_sPtr &device = memoryDevices[devIdx];

        if (device)
        {
            CMem::BlockStatus errCode = device->DiskWrite(bVolNum, ullSectorStart, ulSectorCount, pBuffer);
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
        const CMem::BlockDevice_sPtr &device = memoryDevices[devIdx];

        if (device)
        {
            CMem::BlockStatus errCode = device->DiskFlush(bVolNum);
            ret = static_cast<REDSTATUS>(errCode);
        }
    }

    return ret;
}
#endif /* !REDCONF_READ_ONLY == 0 */

#ifdef __cplusplus
}
#endif
