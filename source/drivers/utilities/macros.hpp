/********************************************************************************
 *  File Name:
 *    macros.hpp
 *
 *  Description:
 *    Helpful macros for Chimera
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_MACROS_HPP
#define CHIMERA_MACROS_HPP

/* STL Includes */
#include <cstddef>
#include <cstring>
#include <type_traits>

/*-------------------------------------------------------------------------------
Macros to Determine Array Count & Size
-------------------------------------------------------------------------------*/
/*-------------------------------------------------
Helps determine if parameter is an array
@see: https://stackoverflow.com/questions/37538/how-do-i-determine-the-size-of-my-array-in-c
-------------------------------------------------*/
#define Static_assert_eval0( e, ... )                    \
  ( 0 * ( int )sizeof( struct {                          \
      _Static_assert( ( e )__VA_OPT__(, ) __VA_ARGS__ ); \
      char ISO_C_forbids_a_struct_with_no_members__;     \
    } ) )
#define is_same_type( a, b ) __builtin_types_compatible_p( typeof( a ), typeof( b ) )
#define is_array( a ) ( !is_same_type( ( a ), &( a )[ 0 ] ) )
#define Static_assert_eval0_array( a ) Static_assert_eval0( is_array( a ), "Not a `[]` !" )

/*-------------------------------------------------
Calculates the number of elements in a C-style array
-------------------------------------------------*/
#define ARRAY_COUNT( arr ) ( ( sizeof( arr ) / sizeof( ( arr )[ 0 ] ) ) )

/*-------------------------------------------------
SIGNED number of elements in the array
-------------------------------------------------*/
#define ARRAY_SCOUNT( arr ) ( ( ptrdiff_t )ARRAY_COUNT( arr ) )

/*-------------------------------------------------
Calculates the number of bytes contained in the array
-------------------------------------------------*/
#define ARRAY_BYTES( arr ) ( sizeof( ( arr )[ 0 ] ) * ARRAY_COUNT( arr ) )


/*-------------------------------------------------------------------------------
Macros for Memory Initialization
-------------------------------------------------------------------------------*/
#define CLEAR_ARRAY( arr ) ( memset( arr, 0, ARRAY_BYTES( arr ) ) )

#endif  /* !CHIMERA_MACROS_HPP */
