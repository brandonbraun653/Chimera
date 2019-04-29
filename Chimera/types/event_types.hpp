/********************************************************************************
 *   File Name:
 *    serial_types.hpp
 *
 *   Description:
 *    Chimera Serial types
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_EVENT_TYPES_HPP
#define CHIMERA_EVENT_TYPES_HPP

/* C++ Includes */
#include <cstdint>
#include <memory>

namespace Chimera::Event
{
  class Notifier;
  using Notifier_sPtr = std::shared_ptr<Notifier>;
  using Notifier_uPtr = std::unique_ptr<Notifier>;
  

  enum class Trigger_t : uint8_t
  {
    READ_COMPLETE = 0,
    WRITE_COMPLETE
  };

}  // namespace Chimera::Serial

#endif /* !CHIMERA_EVENT_TYPES_HPP */