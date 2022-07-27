/********************************************************************************
 *  File Name:
 *    assert_driver.hpp
 *
 *  Description:
 *    Assertion functions for verification of system behavior
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_ASSERT_HPP
#define CHIMERA_ASSERT_HPP

/* STL Includes */
#include <cstdint>
#include <cstddef>
#include <string_view>

/* Chimera Includes */
#include <Chimera/source/drivers/assert/assert_types.hpp>

namespace Chimera::Assert
{

  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  /**
   *  Produces a system reset if the assertion is false
   *
   *  @param[in]  assertion         The assertion to check
   *  @return void
   */
  void hardAssert( const bool assertion, const char *file, const uint32_t line );

  /**
   *  Verifies that an assertion is true at runtime, with the option of also
   *  including a file location for logging.
   *
   *  @param[in]  assertion         The statement being checked
   *  @param[in]  flags             How the assert should behave
   *  @param[in]  line              Line the assert occurred on
   *  @param[in]  file              Which file the assert occurred on
   *  @return void
   */
  void logRuntimeAssert( const bool assertion, const uint8_t flags, const size_t line, const std::string_view file );

}  // namespace Chimera::Assert


/*-------------------------------------------------------------------------------
Macros
-------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------
Create the __SHORT_FILE__ macro, which returns just the file name instead of the
whole path from __FILE__ https://blog.galowicz.de/2016/02/20/short_file_macro/
-------------------------------------------------------------------------------*/
using ccstr = const char *const;
static constexpr ccstr chimera_past_last_slash( ccstr str, ccstr last_slash )
{
  return *str == '\0' ? last_slash : *str == '/' ? chimera_past_last_slash( str + 1, str + 1 ) : chimera_past_last_slash( str + 1, last_slash );
}

static constexpr ccstr chimera_past_last_slash( ccstr str )
{
  return chimera_past_last_slash( str, str );
}
#define __SHORTFILE__                                            \
  ( {                                                            \
    constexpr ccstr sf__{ chimera_past_last_slash( __FILE__ ) }; \
    sf__;                                                        \
  } )


/**
 *  Runtime assertion that will produce a system reset if the assertion fails
 */
#define RT_HARD_ASSERT( statement ) ::Chimera::Assert::hardAssert( ( statement ), __SHORTFILE__, __LINE__ )

#if defined( DEBUG ) && ( DEBUG == 1 )
#define RT_DBG_ASSERT( statement ) RT_HARD_ASSERT( statement )
#else
#define RT_DBG_ASSERT( statement )
#endif /* DEBUG */

#endif /* !CHIMERA_ASSERT_HPP */
