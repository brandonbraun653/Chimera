/********************************************************************************
*  File Name:
*    types.hpp
*
*  Description:
*    
*
*  2020 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/

#ifndef CHIMERA_THREADING_COMMON_TYPES_HPP
#define CHIMERA_THREADING_COMMON_TYPES_HPP

namespace Chimera::Threading
{
  /*------------------------------------------------
  Type Aliases
  ------------------------------------------------*/
  using ThreadArg = void *;
  using ThreadFunctPtr = void ( * )( ThreadArg );
  
  /*------------------------------------------------
  Enumerations
  ------------------------------------------------*/
  enum class Priority
  {
    LEVEL_0, /**< Lowest priority, indicating a task that doesn't care if it runs */
    LEVEL_1,
    LEVEL_2,
    LEVEL_3,
    LEVEL_4,
    LEVEL_5, /**< Highest priority, must always run when requested */

    MINIMUM = LEVEL_0,
    MAXIMUM = LEVEL_5
  };

  /*------------------------------------------------
  Forward declarations
  ------------------------------------------------*/

}

#endif	/* !CHIMERA_THREADING_COMMON_TYPES_HPP */
