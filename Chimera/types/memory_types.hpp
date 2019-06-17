/********************************************************************************
 *   File Name:
 *    memory_types.hpp
 *
 *   Description:
 *    Chimera Memory types
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_DMA_TYPES_HPP
#define CHIMERA_DMA_TYPES_HPP

/* C++ Includes */
#include <cstdint>
#include <memory>

/* Chimera Includes */
#include <Chimera/types/common_types.hpp>

namespace Chimera::Memory
{
  class SystemFlash;
  using SystemFlash_sPtr = std::shared_ptr<SystemFlash>;
  using SystemFlash_uPtr = std::unique_ptr<SystemFlash>;

  class SystemSRAM;
  using SystemSRAM_sPtr = std::shared_ptr<SystemSRAM>;
  using SystemSRAM_uPtr = std::unique_ptr<SystemSRAM>;
}  // namespace Chimera::Memory

#endif /* !CHIMERA_DMA_TYPES_HPP */