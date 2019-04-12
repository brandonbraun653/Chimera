/********************************************************************************
 *  File Name:
 *    flash.hpp
 *
 *  Description:
 *    GMock implementation of the Chimera flash memory interface
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_MOCK_MOD_MEMORY_FLASH_HPP
#define CHIMERA_MOCK_MOD_MEMORY_FLASH_HPP

/* Chimera Includes */
#include <Chimera/modules/memory/flash.hpp>

/* GMock Includes */
#include <gmock/gmock.h>

namespace Chimera
{
  namespace Mock
  {
    namespace Module
    {
      namespace Memory
      {
        class FlashMock : public Chimera::Module::Memory::GenericFlashInterface
        {
          MOCK_METHOD3( write, Chimera::Status_t( const uint32_t, const uint8_t *const, const uint32_t ) );
          MOCK_METHOD3( read, Chimera::Status_t( const uint32_t, uint8_t *const, const uint32_t ) );
          MOCK_METHOD2( erase, Chimera::Status_t( const uint32_t, const uint32_t ) );
          MOCK_METHOD1( writeCompleteCallback, Chimera::Status_t( const Chimera::void_func_uint32_t ) );
          MOCK_METHOD1( readCompleteCallback, Chimera::Status_t( const Chimera::void_func_uint32_t ) );
          MOCK_METHOD1( eraseCompleteCallback, Chimera::Status_t( const Chimera::void_func_uint32_t ) );
        };
      }  // namespace Memory
    }    // namespace Module
  }      // namespace Mock
}  // namespace Chimera

#endif /* !CHIMERA_MOCK_MOD_MEMORY_FLASH_HPP */