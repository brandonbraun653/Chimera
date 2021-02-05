/********************************************************************************
 *  File Name:
 *    scheduler_low_res.cpp
 *
 *  Description:
 *    Implements the software timer based low resolution scheduler
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Aurora Includes */
#include <Aurora/constants>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/function>
#include <Chimera/scheduler>
#include <Chimera/system>
#include <Chimera/thread>


namespace Chimera::Scheduler::LoRes
{
  /*-------------------------------------------------------------------------------
  Constants
  -------------------------------------------------------------------------------*/
  static constexpr size_t s_ThreadStackBytes = 2048;
  static constexpr size_t s_NumTimers        = 15;


  /*-------------------------------------------------------------------------------
  Static Functions
  -------------------------------------------------------------------------------*/
  static void TimerThreadFunction( void *arg );
  static SoftwareTimerEntry *findNextSlot();


  /*-------------------------------------------------------------------------------
  Static Data
  -------------------------------------------------------------------------------*/
  static bool s_CanExecute = false;
  static bool s_CancelThis = false;
  static size_t s_driver_initialized;
  static Chimera::Threading::RecursiveMutex s_mtx;
  static Chimera::Threading::Thread s_TimerThread;
  static SoftwareTimerEntry s_registry[ s_NumTimers ];


  /*-------------------------------------------------------------------------------
  Driver Implementation
  -------------------------------------------------------------------------------*/
  Chimera::Status_t open()
  {
    using namespace Chimera::Threading;

    /*------------------------------------------------
    Prevent multiple initializations (need reset first)
    ------------------------------------------------*/
    if ( s_driver_initialized == Chimera::DRIVER_INITIALIZED_KEY )
    {
      return Chimera::Status::OK;
    }

    s_mtx.lock();

    /*-------------------------------------------------
    Initialize the registry
    -------------------------------------------------*/
    for ( auto x = 0; x < s_NumTimers; x++ )
    {
      s_registry[ x ].clear();
    }

    /*-------------------------------------------------
    Initialize static variables
    -------------------------------------------------*/
    s_CanExecute = true;
    s_CancelThis = false;

    /*-------------------------------------------------
    Initialize the timer thread
    -------------------------------------------------*/
    if ( !s_TimerThread )
    {
      s_TimerThread.initialize( TimerThreadFunction, nullptr, Priority::LOW, STACK_BYTES( s_ThreadStackBytes ), "SWTimer" );
      s_TimerThread.start();
    }

    /*-------------------------------------------------
    Mark as initialized and exit
    -------------------------------------------------*/
    s_driver_initialized = Chimera::DRIVER_INITIALIZED_KEY;
    s_mtx.unlock();

    return Chimera::Status::OK;
  }


  Chimera::Status_t close()
  {
    s_mtx.lock();
    s_CanExecute = false;
    s_mtx.unlock();

    return Chimera::Status::OK;
  }


  size_t resolution()
  {
    return Chimera::Threading::schedulerResolution();
  }


  Chimera::Status_t oneShot( Chimera::Function::Opaque method, const size_t when, const TimingType relation )
  {
    auto result = Chimera::Status::OK;

    /*-------------------------------------------------
    Prevent interruption from threads and ISRs
    -------------------------------------------------*/
    s_mtx.lock();
    auto isrMask = Chimera::System::disableInterrupts();

    /*-------------------------------------------------
    Register the handler
    -------------------------------------------------*/
    if ( auto nextSlot = findNextSlot(); nextSlot )
    {
      /*-------------------------------------------------
      Set up the basic call structure
      -------------------------------------------------*/
      nextSlot->clear();
      nextSlot->callType = CallType::ONE_SHOT;
      nextSlot->func     = method;

      /*-------------------------------------------------
      Set up the call timing
      -------------------------------------------------*/
      if ( relation == TimingType::ABSOLUTE )
      {
        nextSlot->nextCallTime = when;
      }
      else  // TimingType::RELATIVE
      {
        nextSlot->nextCallTime = Chimera::millis() + when;
      }
    }
    else
    {
      result = Chimera::Status::FULL;
    }

    /*-------------------------------------------------
    Allow interrupts again
    -------------------------------------------------*/
    Chimera::System::enableInterrupts( isrMask );
    s_mtx.unlock();

    return result;
  }


  Chimera::Status_t periodic( Chimera::Function::Opaque method, const size_t rate )
  {
    auto result = Chimera::Status::OK;

    /*-------------------------------------------------
    Prevent interruption from threads and ISRs
    -------------------------------------------------*/
    s_mtx.lock();
    auto isrMask = Chimera::System::disableInterrupts();

    /*-------------------------------------------------
    Register the handler
    -------------------------------------------------*/
    if ( auto nextSlot = findNextSlot(); nextSlot )
    {
      /*-------------------------------------------------
      Set up the basic call structure
      -------------------------------------------------*/
      nextSlot->clear();
      nextSlot->callType     = CallType::PERIODIC;
      nextSlot->func         = method;
      nextSlot->callRate     = rate;
      nextSlot->nextCallTime = Chimera::millis() + rate;
    }
    else
    {
      result = Chimera::Status::FULL;
    }

    /*-------------------------------------------------
    Allow interrupts again
    -------------------------------------------------*/
    Chimera::System::enableInterrupts( isrMask );
    s_mtx.unlock();

    return result;
  }


  Chimera::Status_t periodic( Chimera::Function::Opaque method, const size_t rate, const size_t numTimes )
  {
    auto result = Chimera::Status::OK;

    /*-------------------------------------------------
    Prevent interruption from threads and ISRs
    -------------------------------------------------*/
    s_mtx.lock();
    auto isrMask = Chimera::System::disableInterrupts();

    /*-------------------------------------------------
    Register the handler
    -------------------------------------------------*/
    if ( auto nextSlot = findNextSlot(); nextSlot )
    {
      /*-------------------------------------------------
      Set up the basic call structure
      -------------------------------------------------*/
      nextSlot->clear();
      nextSlot->callType     = CallType::PERIODIC_LIMITED;
      nextSlot->func         = method;
      nextSlot->callRate     = rate;
      nextSlot->nextCallTime = Chimera::millis() + rate;
      nextSlot->maxCalls     = numTimes;
      nextSlot->numCalls     = 0;
    }
    else
    {
      result = Chimera::Status::FULL;
    }

    /*-------------------------------------------------
    Allow interrupts again
    -------------------------------------------------*/
    Chimera::System::enableInterrupts( isrMask );
    s_mtx.unlock();

    return result;
  }


  Chimera::Status_t cancel( Chimera::Function::Opaque method )
  {
    auto result = Chimera::Status::NOT_FOUND;
    s_mtx.lock();

    for ( auto timer = 0; timer < s_NumTimers; timer++ )
    {
      if ( s_registry[ timer ].func == method )
      {
        s_registry[ timer ].clear();
        result = Chimera::Status::OK;
        break;
      }
    }

    s_mtx.unlock();
    return result;
  }


  void cancel_this()
  {
    s_CancelThis = true;
  }


  /*-------------------------------------------------------------------------------
  Static Function Definition
  -------------------------------------------------------------------------------*/
  static void TimerThreadFunction( void *arg )
  {
    using namespace Chimera::Threading;

    while ( 1 )
    {
      /*-------------------------------------------------
      Yield execution to other threads if we can't execute
      -------------------------------------------------*/
      if ( !s_CanExecute )
      {
        this_thread::yield();
      }

      /*-------------------------------------------------
      Check if any functions are ready to execute
      -------------------------------------------------*/
      s_mtx.lock();

      for ( auto timer = 0; timer < s_NumTimers; timer++ )
      {
        size_t currentTick = Chimera::millis();

        /*-------------------------------------------------
        Entrance checks
        -------------------------------------------------*/
        if ( !s_registry[ timer ].func || ( s_registry[ timer ].nextCallTime > currentTick ) )
        {
          continue;
        }

        /*-------------------------------------------------
        Execute the function
        -------------------------------------------------*/
        s_registry[ timer ].func();
        s_registry[ timer ].numCalls++;

        /*-------------------------------------------------
        Update the execution state based on the call type
        -------------------------------------------------*/
        switch ( s_registry[ timer ].callType )
        {
          case CallType::PERIODIC:
            s_registry[ timer ].nextCallTime = currentTick + s_registry[ timer ].callRate;
            break;

          case CallType::PERIODIC_LIMITED:
            if ( s_registry[ timer ].numCalls > s_registry[ timer ].maxCalls )
            {
              s_registry[ timer ].clear();
            }
            else
            {
              s_registry[ timer ].nextCallTime = currentTick + s_registry[ timer ].callRate;
            }
            break;

          case CallType::ONE_SHOT:
          default:
            s_registry[ timer ].clear();
            break;
        };

        /*-------------------------------------------------
        Optionally cancel execution of the current function
        -------------------------------------------------*/
        if ( s_CancelThis )
        {
          s_CancelThis = false;
          s_registry[ timer ].clear();
        }
      }

      s_mtx.unlock();

      /*-------------------------------------------------
      Yield execution as there is nothing left to do
      -------------------------------------------------*/
      this_thread::yield();
    }
  }


  static SoftwareTimerEntry *findNextSlot()
  {
    for ( auto timer = 0; timer < s_NumTimers; timer++ )
    {
      if ( s_registry[ timer ].func )
      {
        continue;
      }
      else
      {
        return &s_registry[ timer ];
      }
    }

    return nullptr;
  }

}  // namespace Chimera::Scheduler::LoRes
