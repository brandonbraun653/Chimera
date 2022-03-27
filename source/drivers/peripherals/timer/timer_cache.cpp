/******************************************************************************
 *  File Name:
 *    timer_cache.cpp
 *
 *  Description:
 *    Object caching for timer peripherals. There are so many available driver
 *
 *  2022 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Aurora/container>
#include <Chimera/cfg>
#include <Chimera/common>
#include <Chimera/timer>

namespace Chimera::Timer::Factory
{
  /*---------------------------------------------------------------------------
  Structures
  ---------------------------------------------------------------------------*/
  struct TimerView
  {
    Aurora::Container::List list;
    void                   *view; /**< Instance of the timer interface */
    TimerInterface          type; /**< What type of interface is instantiated */
  };

  /*---------------------------------------------------------------------------
  Static Data
  ---------------------------------------------------------------------------*/
  static Aurora::Container::List s_timer_views[ EnumValue( Instance::NUM_OPTIONS ) ];

  void *build( const TimerInterface type, const Instance periph )
  {
    // Initialize the linked lists
    // Create a new object at runtime on the heap
    // Insert into the list
    // Return the view
  }

}  // namespace Chimera::Timer
