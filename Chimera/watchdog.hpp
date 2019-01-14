/********************************************************************************
*   File Name:
*       watchdog.hpp
*
*   Description:
*       Provides the interface for generic watchdog functionality
*
*   2019 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/

#pragma once
#ifndef CHIMERA_WATCHDOG_HPP
#define CHIMERA_WATCHDOG_HPP

/* C++ Includes */
#include <cstdint>
#include <memory>
#include <type_traits>

/** @namespace Chimera */
namespace Chimera
{
    /* @namespace Watchdog */
    namespace Watchdog
    {
        class Interface
        {
        public:
            
            virtual void initialize() = 0;

            virtual void start() = 0;

            virtual void stop() = 0; 

            virtual void kick() = 0;

            virtual ~Interface() = default;
        }

        class WatchdogHW : public CHIMERA_INHERITED_WATCHDOG
        {
        public:
            WatchdogHW() = default;
            ~WatchdogHW() = default;
        };

        static_assert(std::is_base_of<Interface, WatchdogHW>::value, 
            "CHIMERA: Watchdog base class does not implement the correct interface");

        typedef std::shared_ptr<Chimera::Watchdog::WatchdogHW> WatchdogHW_sPtr;
        typedef std::unique_ptr<Chimera::Watchdog::WatchdogHW> WatchdogHW_uPtr;

    } // Watchdog
} // Chimera


#endif /* !CHIMERA_WATCHDOG_HPP */