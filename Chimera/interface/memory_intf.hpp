/********************************************************************************
 *   File Name:
 *    memory_intf.hpp
 *
 *   Description:
 *    Models the Chimera memory interface 
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once 
#ifndef CHIMERA_MEMORY_INTERFACE_HPP
#define CHIMERA_MEMORY_INTERFACE_HPP

/* C++ Includes */
#include <cstdint>

/* Chimera Includes */
#include <Chimera/types/common_types.hpp>
#include <Chimera/modules/memory/device.hpp>

namespace Chimera
{
 namespace Memory
  {
    class SystemFlashUnsupported : public Chimera::Modules::Memory::GenericInterface
    {
    public:
      Chimera::Status_t write( const uint32_t address, const uint8_t *const data, const uint32_t length ) final override
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }

      Chimera::Status_t read( const uint32_t address, uint8_t *const data, const uint32_t length ) final override
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }

      Chimera::Status_t erase( const uint32_t address, const uint32_t length ) final override
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }

      Chimera::Status_t writeCompleteCallback( const Chimera::Function::void_func_uint32_t func ) final override
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }

      Chimera::Status_t readCompleteCallback( const Chimera::Function::void_func_uint32_t func ) final override
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }

      Chimera::Status_t eraseCompleteCallback( const Chimera::Function::void_func_uint32_t func ) final override
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }

      bool isInitialized() final override
      {
        return false;
      }
    };

    class SystemSRAMUnsupported : public Chimera::Modules::Memory::GenericInterface
    {
    public:
      Chimera::Status_t write( const uint32_t address, const uint8_t *const data, const uint32_t length ) final override
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }

      Chimera::Status_t read( const uint32_t address, uint8_t *const data, const uint32_t length ) final override
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }

      Chimera::Status_t erase( const uint32_t address, const uint32_t length ) final override
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }

      Chimera::Status_t writeCompleteCallback( const Chimera::Function::void_func_uint32_t func ) final override
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }

      Chimera::Status_t readCompleteCallback( const Chimera::Function::void_func_uint32_t func ) final override
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }

      Chimera::Status_t eraseCompleteCallback( const Chimera::Function::void_func_uint32_t func ) final override
      {
        return Chimera::CommonStatusCodes::NOT_SUPPORTED;
      }

      bool isInitialized() final override
      {
        return false;
      }
    };
  }    // namespace Memory
}  // namespace Chimera

#endif /* !CHIMERA_MEMORY_INTERFACE_HPP */