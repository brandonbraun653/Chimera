/********************************************************************************
 * File Name:
 *
 *
 * Description:
 *
 *
 * 2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#include <Chimera/modules/memory/flash.hpp>


namespace Chimera
{
  namespace Modules
  {
    namespace Memory
    {
      MemoryBlockRange::MemoryBlockRange( const uint32_t startAddress, const uint32_t endAddress, const uint32_t blockSize )
      {
        if ( ( startAddress < endAddress ) && ( blockSize > 0 ) )
        {
          _blockSize    = blockSize;
          _endAddress   = endAddress;
          _startAddress = startAddress;
          initialized   = true;
        }
      }

      MemoryBlockRange::MemoryBlockRange( const MemoryBlockRange &cls )
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

      uint32_t MemoryBlockRange::startBlock()
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

      uint32_t MemoryBlockRange::startOffset()
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

      uint32_t MemoryBlockRange::startBytes()
      {
        uint32_t retVal = U32MAX;

        if ( initialized )
        {
          if ( _startBytes == U32MAX )
          {
            uint32_t nextBlockBeginAddress = ( startBlock() + 1 ) * _blockSize;
            _startBytes                    = nextBlockBeginAddress - _startAddress;
          }

          retVal = _startBytes;
        }

        return retVal;
      }

      uint32_t MemoryBlockRange::endBlock()
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

      uint32_t MemoryBlockRange::endOffset()
      {
        uint32_t retVal = U32MAX;

        if ( initialized )
        {
          if ( _endOffset == U32MAX )
          {
            uint32_t endBlockBeginAddress = endBlock() * _blockSize;
            _endOffset                    = _endAddress - endBlockBeginAddress;
          }

          retVal = _endOffset;
        }

        return retVal;
      }

      uint32_t MemoryBlockRange::endBytes()
      {
        uint32_t retVal = U32MAX;

        if ( initialized )
        {
          if ( _endBytes == U32MAX )
          {
            uint32_t nextBlockBeginAddress = ( endBlock() + 1 )* _blockSize;
            _endBytes                      = nextBlockBeginAddress - _endAddress;
          }

          retVal = _endBytes;
        }

        return retVal;
      }

      void MemoryBlockRange::getConsecutiveBlocks( uint32_t &start, uint32_t &end )
      {
        if ( initialized )
        {
          start = startBlock();
          if ( startOffset() )
          {
            start += 1u;
          }

          end = endBlock();
          if ( ( end != start ) && endOffset() )
          {
            end -= 1u;
          }
        }
        else
        {
          start = U32MAX;
          end   = U32MAX;
        }
      }
    }  // namespace Memory
  }    // namespace Modules
}  // namespace Chimera
