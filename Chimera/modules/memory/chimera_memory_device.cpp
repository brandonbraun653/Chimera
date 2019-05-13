/********************************************************************************
 *  File Name:
 *    chimera_memory_device.cpp
 *
 *  Description:
 *    Implements various utilities and drivers associated with the Chimera memory
 *    module.
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */
#include <cstring>

/* Chimera Includes */
#include <Chimera/modules/memory/device.hpp>

namespace Chimera::Modules::Memory
{
  BlockRange::BlockRange( const size_t startAddress, const size_t endAddress, const size_t blockSize )
  {
    if ( ( startAddress < endAddress ) && ( blockSize > 0 ) )
    {
      _blockSize    = blockSize;
      _endAddress   = endAddress;
      _startAddress = startAddress;
      initialized   = true;
    }
  }

  BlockRange::BlockRange( const BlockRange &cls )
  {
    this->_blockSize = cls._blockSize;

    this->_startBlock   = cls._startBlock;
    this->_startAddress = cls._startAddress;
    this->_startOffset  = cls._startOffset;

    this->_endBlock   = cls._endBlock;
    this->_endAddress = cls._endAddress;
    this->_endOffset  = cls._endOffset;

    this->initialized = cls.initialized;
  }

  size_t BlockRange::startBlock()
  {
    size_t retVal = SIZE_T_MAX;

    if ( initialized )
    {
      if ( _startBlock == SIZE_T_MAX )
      {
        _startBlock = _startAddress / _blockSize;
      }

      retVal = _startBlock;
    }

    return retVal;
  }

  size_t BlockRange::startOffset()
  {
    size_t retVal = SIZE_T_MAX;

    if ( initialized )
    {
      if ( _startOffset == SIZE_T_MAX )
      {
        size_t startBlockBeginAddress = startBlock() * _blockSize;
        _startOffset                    = _startAddress - startBlockBeginAddress;
      }

      retVal = _startOffset;
    }

    return retVal;
  }

  size_t BlockRange::startBytes()
  {
    size_t retVal = SIZE_T_MAX;

    if ( initialized )
    {
      if ( _startBytes == SIZE_T_MAX )
      {
        size_t memoryRange           = _endAddress - _startAddress;
        size_t nextBlockBeginAddress = ( startBlock() + 1 ) * _blockSize;

        /*------------------------------------------------
        If the total number of bytes in the range doesn't exceed the next block
        boundary address, return the range. Otherwise return the number of bytes
        up to the boundary address.
        ------------------------------------------------*/
        if ( ( startOffset() + memoryRange ) < _blockSize )
        {
          _startBytes = memoryRange;
        }
        else
        {
          _startBytes = nextBlockBeginAddress - _startAddress;
        }
      }

      retVal = _startBytes;
    }

    return retVal;
  }

  size_t BlockRange::endBlock()
  {
    size_t retVal = SIZE_T_MAX;

    if ( initialized )
    {
      if ( _endBlock == SIZE_T_MAX )
      {
        _endBlock = _endAddress / _blockSize;
      }

      retVal = _endBlock;
    }

    return retVal;
  }

  size_t BlockRange::endOffset()
  {
    size_t retVal = SIZE_T_MAX;

    if ( initialized )
    {
      if ( _endOffset == SIZE_T_MAX )
      {
        if ( endBlock() == startBlock() )
        {
          _endOffset = 0u;
        }
        else
        {
          _endOffset = _endAddress - ( endBlock() * _blockSize );
        }
      }

      retVal = _endOffset;
    }

    return retVal;
  }

  size_t BlockRange::endBytes()
  {
    size_t retVal = SIZE_T_MAX;

    if ( initialized )
    {
      if ( _endBytes == SIZE_T_MAX )
      {
        size_t nextBlockBeginAddress = ( endBlock() + 1 ) * _blockSize;
        _endBytes                      = nextBlockBeginAddress - _endAddress;
      }

      retVal = _endBytes;
    }

    return retVal;
  }


  Utilities::Utilities( const Descriptor &dev ) : device( dev )
  {
    pagesPerBlock   = 0u;
    pagesPerSector  = 0u;
    blocksPerSector = 0u;

    updateDeviceInfo( dev );
  }

  void Utilities::updateDeviceInfo( const Descriptor &dev )
  {
    device = dev;

    // assert( device.pageSize != 0 );
    // assert( device.blockSize != 0 );
    // assert( device.sectorSize != 0 );

    pagesPerBlock   = device.blockSize / device.pageSize;
    pagesPerSector  = device.sectorSize / device.pageSize;
    blocksPerSector = device.sectorSize / device.blockSize;
  }

  size_t Utilities::getSectionNumber( const Section_t section, const size_t address )
  {
    size_t sectionNumber = std::numeric_limits<size_t>::max();

    switch ( section )
    {
      case Section_t::PAGE:
        // assert( device.pageSize != 0 );
        sectionNumber = address / device.pageSize;
        break;

      case Section_t::BLOCK:
        // assert( device.blockSize != 0 );
        sectionNumber = address / device.blockSize;
        break;

      case Section_t::SECTOR:
        // assert( device.sectorSize != 0 );
        sectionNumber = address / device.sectorSize;
        break;

      default:
        break;
    };

    return sectionNumber;
  }

  size_t Utilities::getSectionStartAddress( const Section_t section, const size_t number )
  {
    size_t address = std::numeric_limits<size_t>::max();

    switch ( section )
    {
      case Section_t::PAGE:
        address = number * device.pageSize;
        break;

      case Section_t::BLOCK:
        address = number * device.blockSize;
        break;

      case Section_t::SECTOR:
        address = number * device.sectorSize;
        break;

      default:
        break;
    };

    return address;
  }

  SectionList Utilities::getCompositeSections( const size_t address, const size_t len )
  {
    SectionList section;
    BlockRange pageRange( address, address + len, device.pageSize );

    /*------------------------------------------------
    Initialize the algorithm, performing the first
    iteration manually.
    ------------------------------------------------*/
    size_t numPages = pageRange.endBlock() - pageRange.startBlock();

    size_t currentBlock      = getSectionNumber( Section_t::BLOCK, address );
    size_t lastBlock         = currentBlock;
    size_t consecutiveBlocks = 1;

    size_t currentSector      = getSectionNumber( Section_t::SECTOR, address );
    size_t lastSector         = currentSector;
    size_t consecutiveSectors = 1;

    size_t currentPage = pageRange.startBlock() + 1;
    section.pages.push_back( pageRange.startBlock() );

    /*------------------------------------------------
    Iterate through the remaining pages and collapse them into
    blocks and sectors as they pass the threshold for their
    respective memory boundaries.
    ------------------------------------------------*/
    for ( size_t x = 1; x < numPages; x++ )
    {
      size_t currentAddress = currentPage * device.pageSize;
      currentBlock            = getSectionNumber( Section_t::BLOCK, currentAddress );
      currentSector           = getSectionNumber( Section_t::SECTOR, currentAddress );

      /*------------------------------------------------
      Update page tracking metrics. This is the smallest
      memory unit, so there isn't much to do here.
      ------------------------------------------------*/
      section.pages.push_back( currentPage );

      /*------------------------------------------------
      Update block tracking metrics
      ------------------------------------------------*/
      if ( currentBlock == lastBlock )
      {
        consecutiveBlocks++;
      }
      else
      {
        consecutiveBlocks = 1;
        lastBlock         = currentBlock;
      }

      if ( consecutiveBlocks == pagesPerBlock )
      {
        /*------------------------------------------------
        Consolidate the accrued consecutive pages into a single block
        ------------------------------------------------*/
        section.blocks.push_back( currentBlock );
        section.pages.resize( section.pages.size() - pagesPerBlock );

        consecutiveBlocks = 1;
      }

      /*------------------------------------------------
      Update sector tracking metrics
      ------------------------------------------------*/
      if ( currentSector == lastSector )
      {
        consecutiveSectors++;
      }
      else
      {
        consecutiveSectors = 1;
        lastSector         = currentSector;
      }

      if ( consecutiveSectors == pagesPerSector )
      {
        /*------------------------------------------------
        Consolidate the accrued consecutive blocks into a single sector
        ------------------------------------------------*/
        section.sectors.push_back( currentSector );
        section.blocks.resize( section.blocks.size() - blocksPerSector );

        consecutiveSectors = 1;
      }

      /*------------------------------------------------
      Update the actual page we are at
      ------------------------------------------------*/
      currentPage++;
    }

    return section;
  }


  Chimera::Status_t VirtualMemoryDevice::write( const size_t address, const uint8_t *const data, const size_t length )
  {
    Chimera::Status_t result = Chimera::CommonStatusCodes::OK;
    auto writeAddr           = rawData + address;
    auto endAddr             = reinterpret_cast<uintptr_t>( writeAddr ) + length;

    if ( !data || ( endAddr > deviceDescriptor.endAddress ) || ( address > regionSize )  )
    {
      result = Chimera::CommonStatusCodes::INVAL_FUNC_PARAM;
    }
    else
    {
      memcpy( writeAddr, data, length );
    }

    return result;
  }

  Chimera::Status_t VirtualMemoryDevice::read( const size_t address, uint8_t *const data, const size_t length )
  {
    Chimera::Status_t result = Chimera::CommonStatusCodes::OK;
    auto writeAddr           = rawData + address;
    auto endAddr             = reinterpret_cast<uintptr_t>( writeAddr ) + length;

    if ( !data || ( endAddr > deviceDescriptor.endAddress ) || ( address > regionSize) )
    {
      result = Chimera::CommonStatusCodes::INVAL_FUNC_PARAM;
    }
    else
    {
      memcpy( data, writeAddr, length );
    }

    return result;
  }

  Chimera::Status_t VirtualMemoryDevice::erase( const size_t address, const size_t length )
  {
    Chimera::Status_t result = Chimera::CommonStatusCodes::OK;
    auto writeAddr           = rawData + address;
    auto endAddr             = reinterpret_cast<uintptr_t>( writeAddr ) + length;

    if ( ( endAddr > deviceDescriptor.endAddress ) || ( address > regionSize ) )
    {
      result = Chimera::CommonStatusCodes::INVAL_FUNC_PARAM;
    }
    else
    {
      memset( writeAddr, 0x00, length );
    }

    return result;
  }

  Chimera::Status_t VirtualMemoryDevice::writeCompleteCallback( const Chimera::Function::void_func_uint32_t func )
  {
    return Chimera::CommonStatusCodes::NOT_SUPPORTED;
  }

  Chimera::Status_t VirtualMemoryDevice::readCompleteCallback( const Chimera::Function::void_func_uint32_t func )
  {
    return Chimera::CommonStatusCodes::NOT_SUPPORTED;
  }

  Chimera::Status_t VirtualMemoryDevice::eraseCompleteCallback( const Chimera::Function::void_func_uint32_t func )
  {
    return Chimera::CommonStatusCodes::NOT_SUPPORTED;
  }

  bool VirtualMemoryDevice::isInitialized()
  {
    return initialized;
  }

}  // namespace Chimera::Modules::Memory
