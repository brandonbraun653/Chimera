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

/* Chimera Includes */
#include <Chimera/modules/memory/device.hpp>

namespace Chimera
{
  namespace Modules
  {
    namespace Memory
    {
      BlockRange::BlockRange( const uint32_t startAddress, const uint32_t endAddress, const uint32_t blockSize )
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

      uint32_t BlockRange::startBlock()
      {
        uint32_t retVal = U32MAX;

        if ( initialized )
        {
          if ( _startBlock == U32MAX )
          {
            _startBlock = _startAddress / _blockSize;
          }

          retVal = _startBlock;
        }

        return retVal;
      }

      uint32_t BlockRange::startOffset()
      {
        uint32_t retVal = U32MAX;

        if ( initialized )
        {
          if ( _startOffset == U32MAX )
          {
            uint32_t startBlockBeginAddress = startBlock() * _blockSize;
            _startOffset                    = _startAddress - startBlockBeginAddress;
          }

          retVal = _startOffset;
        }

        return retVal;
      }

      uint32_t BlockRange::startBytes()
      {
        uint32_t retVal = U32MAX;

        if ( initialized )
        {
          if ( _startBytes == U32MAX )
          {
            uint32_t memoryRange           = _endAddress - _startAddress;
            uint32_t nextBlockBeginAddress = ( startBlock() + 1 ) * _blockSize;

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

      uint32_t BlockRange::endBlock()
      {
        uint32_t retVal = U32MAX;

        if ( initialized )
        {
          if ( _endBlock == U32MAX )
          {
            _endBlock = _endAddress / _blockSize;
          }

          retVal = _endBlock;
        }

        return retVal;
      }

      uint32_t BlockRange::endOffset()
      {
        uint32_t retVal = U32MAX;

        if ( initialized )
        {
          if ( _endOffset == U32MAX )
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

      uint32_t BlockRange::endBytes()
      {
        uint32_t retVal = U32MAX;

        if ( initialized )
        {
          if ( _endBytes == U32MAX )
          {
            uint32_t nextBlockBeginAddress = ( endBlock() + 1 ) * _blockSize;
            _endBytes                      = nextBlockBeginAddress - _endAddress;
          }

          retVal = _endBytes;
        }

        return retVal;
      }


      Utilities::Utilities( const DeviceDescriptor &dev ) : device( dev )
      {
        pagesPerBlock   = 0u;
        pagesPerSector  = 0u;
        blocksPerSector = 0u;

        updateDeviceInfo( dev );
      }

      void Utilities::updateDeviceInfo( const DeviceDescriptor &dev )
      {
        device = dev;

        assert( device.pageSize != 0 );
        assert( device.blockSize != 0 );
        assert( device.sectorSize != 0 );

        pagesPerBlock   = device.blockSize / device.pageSize;
        pagesPerSector  = device.sectorSize / device.pageSize;
        blocksPerSector = device.sectorSize / device.blockSize;
      }

      uint32_t Utilities::getSectionNumber( const Section_t section, const uint32_t address )
      {
        uint32_t sectionNumber = std::numeric_limits<uint32_t>::max();

        switch ( section )
        {
          case Section_t::PAGE:
            assert( device.pageSize != 0 );
            sectionNumber = address / device.pageSize;
            break;

          case Section_t::BLOCK:
            assert( device.blockSize != 0 );
            sectionNumber = address / device.blockSize;
            break;

          case Section_t::SECTOR:
            assert( device.sectorSize != 0 );
            sectionNumber = address / device.sectorSize;
            break;

          default:
            break;
        };

        return sectionNumber;
      }

      uint32_t Utilities::getSectionStartAddress( const Section_t section, const uint32_t number )
      {
        uint32_t address = std::numeric_limits<uint32_t>::max();

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

      SectionList Utilities::getCompositeSections( const uint32_t address, const uint32_t len )
      {
        SectionList section;
        BlockRange pageRange( address, address + len, device.pageSize );

        /*------------------------------------------------
        Initialize the algorithm, performing the first
        iteration manually.
        ------------------------------------------------*/
        uint32_t numPages = pageRange.endBlock() - pageRange.startBlock();

        uint32_t currentBlock      = getSectionNumber( Section_t::BLOCK, address );
        uint32_t lastBlock         = currentBlock;
        uint32_t consecutiveBlocks = 1;

        uint32_t currentSector      = getSectionNumber( Section_t::SECTOR, address );
        uint32_t lastSector         = currentSector;
        uint32_t consecutiveSectors = 1;

        uint32_t currentPage = pageRange.startBlock() + 1;
        section.pages.push_back( pageRange.startBlock() );

        /*------------------------------------------------
        Iterate through the remaining pages and collapse them into
        blocks and sectors as they pass the threshold for their
        respective memory boundaries.
        ------------------------------------------------*/
        for ( uint32_t x = 1; x < numPages; x++ )
        {
          uint32_t currentAddress = currentPage * device.pageSize;
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


    }  // namespace Memory
  }    // namespace Modules
}  // namespace Chimera
