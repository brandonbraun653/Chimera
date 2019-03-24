/********************************************************************************
 *  File Name:
 *    test_chimera_flash.cpp
 *
 *  Description:
 *    Tests various functions associated with the Chimera Flash module
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#include <gtest/gtest.h>

#include <Chimera/modules/memory/flash.hpp>

using namespace Chimera::Modules::Memory;

TEST( FlashUtil_CompositeSections, AlignedMultiBlock_1 )
{
  /*------------------------------------------------
  Initialize
  ------------------------------------------------*/
  static constexpr uint32_t pageSize = 256u;
  static constexpr uint32_t blockSize = 2048u;
  static constexpr uint32_t sectorSize = 65536u;

  DeviceDescriptor dev_binary{ pageSize, blockSize, sectorSize };
  FlashUtilities util( dev_binary );

  static constexpr uint32_t page         = 8;
  static constexpr uint32_t len          = 2 * blockSize;
  static constexpr uint32_t startAddress = page * pageSize;

  std::vector<uint32_t> expectedBlocks = { 1, 2 };

  /*------------------------------------------------
  Call FUT
  ------------------------------------------------*/
  auto sections = util.getCompositeSections( startAddress, len );

  /*------------------------------------------------
  Verify
  ------------------------------------------------*/
  EXPECT_EQ( sections.pages.size(), 0 );
  EXPECT_EQ( sections.blocks.size(), 2 );
  EXPECT_EQ( sections.sectors.size(), 0 );
  EXPECT_EQ( sections.blocks, expectedBlocks );
}

TEST( FlashUtil_CompositeSections, AlignedMultiBlock_2 )
{
  /*------------------------------------------------
  Initialize
  ------------------------------------------------*/
  static constexpr uint32_t pageSize   = 256u;
  static constexpr uint32_t blockSize  = 2048u;
  static constexpr uint32_t sectorSize = 65536u;

  DeviceDescriptor dev_binary{ pageSize, blockSize, sectorSize };
  FlashUtilities util( dev_binary );

  static constexpr uint32_t page         = 8;
  static constexpr uint32_t len          = 135 * blockSize;
  static constexpr uint32_t startAddress = page * pageSize;

  std::vector<uint32_t> expectedBlocks;
  for ( uint32_t x = 1; x <= 31; x++ )
  {
    expectedBlocks.push_back( x );
  }

  for( uint32_t x=128; x<=135; x++ )
  {
    expectedBlocks.push_back( x );
  }

  std::vector<uint32_t> expectedSectors = { 1, 2, 3 };

  /*------------------------------------------------
  Call FUT
  ------------------------------------------------*/
  auto sections = util.getCompositeSections( startAddress, len );

  /*------------------------------------------------
  Verify
  ------------------------------------------------*/
  EXPECT_EQ( sections.pages.size(), 0 );
  EXPECT_EQ( sections.blocks.size(), 39 );
  EXPECT_EQ( sections.sectors.size(), 3 );
  EXPECT_EQ( sections.blocks, expectedBlocks );
  EXPECT_EQ( sections.sectors, expectedSectors );
}

TEST( FlashUtil_CompositeSections, UnAlignedMultiBlock_1 )
{
  /*------------------------------------------------
  Initialize
  ------------------------------------------------*/
  static constexpr uint32_t pageSize   = 256u;
  static constexpr uint32_t blockSize  = 2048u;
  static constexpr uint32_t sectorSize = 65536u;

  DeviceDescriptor dev_binary{ pageSize, blockSize, sectorSize };
  FlashUtilities util( dev_binary );

  static constexpr uint32_t page         = 12;
  static constexpr uint32_t len          = ( 3 * blockSize ) + ( 5 * pageSize );
  static constexpr uint32_t startAddress = page * pageSize;

  std::vector<uint32_t> expectedPages = { 12, 13, 14, 15, 40 };
  std::vector<uint32_t> expectedBlocks = { 2, 3, 4 };

  /*------------------------------------------------
  Call FUT
  ------------------------------------------------*/
  auto sections = util.getCompositeSections( startAddress, len );

  /*------------------------------------------------
  Verify
  ------------------------------------------------*/
  EXPECT_EQ( sections.pages.size(), 5 );
  EXPECT_EQ( sections.blocks.size(), 3 );
  EXPECT_EQ( sections.sectors.size(), 0 );

  EXPECT_EQ( expectedPages, sections.pages );
  EXPECT_EQ( expectedBlocks, sections.blocks );
}

TEST( FlashUtil_CompositeSections, UnAlignedMultiPageBlockSector )
{
  /*------------------------------------------------
  Initialize
  ------------------------------------------------*/
  static constexpr uint32_t pageSize   = 256u;
  static constexpr uint32_t blockSize  = 2048u;
  static constexpr uint32_t sectorSize = 65536u;

  DeviceDescriptor dev_binary{ pageSize, blockSize, sectorSize };
  FlashUtilities util( dev_binary );

  static constexpr uint32_t page         = 12;
  static constexpr uint32_t len          = ( 2 * sectorSize ) + ( 3 * blockSize ) + ( 5 * pageSize );
  static constexpr uint32_t startAddress = page * pageSize;

  std::vector<uint32_t> expectedPages  = { 12, 13, 14, 15, 552 };
  std::vector<uint32_t> expectedBlocks;
  std::vector<uint32_t> expectedSectors = { 1 };

  for ( uint32_t x = 2; x <= 31; x++ )
  {
    expectedBlocks.push_back( x );
  }

  for ( uint32_t x = 64; x <= 68; x++ )
  {
    expectedBlocks.push_back( x );
  }

  /*------------------------------------------------
  Call FUT
  ------------------------------------------------*/
  auto sections = util.getCompositeSections( startAddress, len );

  /*------------------------------------------------
  Verify
  ------------------------------------------------*/
  EXPECT_EQ( sections.pages.size(), 5 );
  EXPECT_EQ( sections.blocks.size(), 35 );
  EXPECT_EQ( sections.sectors.size(), 1 );

  EXPECT_EQ( expectedPages, sections.pages );
  EXPECT_EQ( expectedBlocks, sections.blocks );

  EXPECT_EQ( len, ( ( sections.pages.size() * pageSize ) + ( sections.blocks.size() * blockSize )
                    + ( sections.sectors.size() * sectorSize ) ) );
}

#if defined( GMOCK_TEST )
#include <gmock/gmock.h>

#endif /* GMOCK_TEST */
