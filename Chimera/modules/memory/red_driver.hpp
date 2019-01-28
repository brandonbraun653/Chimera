/********************************************************************************
*   File Name:
*       red_driver.hpp
*
*   Description:
*       Provides a high level C++ API wrapper around the Reliance-Edge FS
*
*   2019 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/

#pragma once
#ifndef CHIMERA_RED_DRIVER_HPP
#define CHIMERA_RED_DRIVER_HPP

/* Reliance Edge FS Includes */
#ifdef __cplusplus
extern "C" {
#include "redfs.h"
}
#endif

/* C++ Includes */
#include <cstdint>
#include <memory>
#include <type_traits>

/* Chimera Includes */
#include <Chimera/modules/memory/blockDevice.hpp>

namespace Red
{
    static constexpr uint8_t MAX_MEMORY_DEVICES = 5;

    class RedFS
    {
    public:

        REDSTATUS init(Chimera::Modules::Memory::BlockDevice_sPtr &device);

    private:

    };
}

#endif /* !CHIMERA_RED_DRIVER_HPP */
