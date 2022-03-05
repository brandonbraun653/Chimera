/******************************************************************************
 *  File Name:
 *    scheduler_polling.cpp
 *
 *  Description:
 *    Polling based scheduled functionality
 *
 *  2022 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/scheduler>

namespace Chimera::Scheduler
{
  /*---------------------------------------------------------------------------
  Class Implementation
  ---------------------------------------------------------------------------*/
  Polled::Polled() : mCB( {} )
  {
  }


  Polled::~Polled()
  {
  }


  int Polled::poll()
  {
    /*-------------------------------------------------------------------------
    Entrance checks
    -------------------------------------------------------------------------*/
    const size_t currentTick = Chimera::millis();
    int call_time            = 0;

    if ( !mCB.func )
    {
      return std::numeric_limits<int>::min();
    }
    else if ( mCB.nextCallTime > currentTick )
    {
      /*-----------------------------------------------------------------------
      Return number of milliseconds we are early
      -----------------------------------------------------------------------*/
      return static_cast<int>( currentTick ) - static_cast<int>( mCB.nextCallTime );
    }
    else
    {
      /*-----------------------------------------------------------------------
      Number of milliseconds after the expected call time
      -----------------------------------------------------------------------*/
      call_time = mCB.nextCallTime - currentTick;
    }

    /*-------------------------------------------------------------------------
    Execute the desired function
    -------------------------------------------------------------------------*/
    mCB.func();
    mCB.numCalls++;

    /*-------------------------------------------------------------------------
    Update the execution state based on the call type
    -------------------------------------------------------------------------*/
    switch ( mCB.callType )
    {
      case CallType::PERIODIC:
        mCB.nextCallTime = currentTick + mCB.callRate;
        break;

      case CallType::PERIODIC_LIMITED:
        if ( mCB.numCalls > mCB.maxCalls )
        {
          mCB.clear();
        }
        else
        {
          mCB.nextCallTime = currentTick + mCB.callRate;
        }
        break;

      case CallType::ONE_SHOT:
      default:
        mCB.clear();
        break;
    };

    return call_time;
  }


  Chimera::Status_t Polled::oneShot( Chimera::Function::Opaque method, const size_t when, const TimingType relation )
  {
    mCB.clear();
    mCB.callType = CallType::ONE_SHOT;
    mCB.func     = method;

    if ( relation == TimingType::ABSOLUTE )
    {
      mCB.nextCallTime = when;
    }
    else  // TimingType::RELATIVE
    {
      mCB.nextCallTime = Chimera::millis() + when;
    }

    return Chimera::Status::OK;
  }


  Chimera::Status_t Polled::periodic( Chimera::Function::Opaque method, const size_t rate )
  {
    mCB.clear();
    mCB.callType     = CallType::PERIODIC;
    mCB.func         = method;
    mCB.callRate     = rate;
    mCB.nextCallTime = Chimera::millis() + rate;
    return Chimera::Status::OK;
  }


  Chimera::Status_t Polled::periodic( Chimera::Function::Opaque method, const size_t rate, const size_t numTimes )
  {
    mCB.clear();
    mCB.callType     = CallType::PERIODIC_LIMITED;
    mCB.func         = method;
    mCB.callRate     = rate;
    mCB.nextCallTime = Chimera::millis() + rate;
    mCB.maxCalls     = numTimes;
    mCB.numCalls     = 0;
    return Chimera::Status::OK;
  }

}  // namespace Chimera::Scheduler
