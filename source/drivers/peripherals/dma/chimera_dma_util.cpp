/********************************************************************************
 *  File Name:
 *    chimera_dma_util.cpp
 *
 *  Description:
 *    DMA utility methods for helping queue transfers, gather statistics, and
 *    generate unique identifiers
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* STL Includes */
#include <cstdint>

/* Aurora Includes */
#include <Aurora/logging>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/dma>
#include <Chimera/thread>

/* ETL Includes */
#include <etl/memory_model.h>
#include <etl/random.h>
#include <etl/queue.h>
#include <etl/unordered_map.h>
#include <etl/unordered_set.h>

/*-------------------------------------------------------------------------------
Literals
-------------------------------------------------------------------------------*/
#ifndef CHIMERA_DMA_MEM_QUEUE_SIZE
#define CHIMERA_DMA_MEM_QUEUE_SIZE ( 15 )
#endif

#ifndef CHIMERA_DMA_PIPE_QUEUE_SIZE
#define CHIMERA_DMA_PIPE_QUEUE_SIZE ( 15 )
#endif


namespace Chimera::DMA::Util
{
  /*-------------------------------------------------------------------------------
  Constants
  -------------------------------------------------------------------------------*/
  static constexpr size_t MAX_UUIDS = CHIMERA_DMA_MEM_QUEUE_SIZE + CHIMERA_DMA_PIPE_QUEUE_SIZE;

  /*-------------------------------------------------------------------------------
  Aliases
  -------------------------------------------------------------------------------*/
  /*-------------------------------------------------
  Using unordered_<map/set> for lookup speed
  -------------------------------------------------*/
  using ReqSet    = etl::unordered_set<RequestId, MAX_UUIDS>;
  using MemMap    = etl::unordered_map<RequestId, MemTransfer, CHIMERA_DMA_MEM_QUEUE_SIZE>;
  using PipeMap   = etl::unordered_map<RequestId, PipeTransfer, CHIMERA_DMA_PIPE_QUEUE_SIZE>;
  using ReqQueue  = etl::queue<RequestId, CHIMERA_DMA_MEM_QUEUE_SIZE, etl::memory_model::MEMORY_MODEL_SMALL>;
  using PipeQueue = etl::queue<RequestId, CHIMERA_DMA_PIPE_QUEUE_SIZE, etl::memory_model::MEMORY_MODEL_SMALL>;

  /*-------------------------------------------------------------------------------
  Static Data
  -------------------------------------------------------------------------------*/
  static Chimera::Thread::RecursiveMutex s_lock;
  static etl::random_xorshift s_rand_gen;
  static ReqSet s_uuid_set;

  /*-------------------------------------------------
  Memory Request Transfers
  -------------------------------------------------*/
  static MemMap s_request_data;
  static ReqQueue s_request_queue;

  /*-------------------------------------------------
  Pipe Transfers
  -------------------------------------------------*/
  static PipeMap s_pipe_data;
  static PipeQueue s_pipe_queue;


  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  RequestId genRequestId()
  {
    using namespace Aurora::Logging;
    using namespace Chimera::Thread;
    LockGuard lck( s_lock );

    /*-------------------------------------------------
    Ensure the set isn't full
    -------------------------------------------------*/
    if( s_uuid_set.full() )
    {
      return INVALID_REQUEST;
    }

    /*-------------------------------------------------
    Generate a UUID that isn't already in the set
    -------------------------------------------------*/
    size_t attempts = 0;
    RequestId result = INVALID_REQUEST;

    while( attempts < 3 )
    {
      RequestId next = s_rand_gen();

      if( s_uuid_set.find( next ) == s_uuid_set.end() )
      {
        s_uuid_set.insert( next );
        result = next;
        break;
      }
      else
      {
        attempts++;
      }
    }
    LOG_ERROR_IF( attempts != 3, "DMA failed to generate UUID\r\n" );

    return result;
  }


  void initializeQueues()
  {
    using namespace Chimera::Thread;
    LockGuard lck( s_lock );

    /*-------------------------------------------------
    Clear out the queues
    -------------------------------------------------*/
    s_request_data.clear();
    s_request_queue.clear();

    s_pipe_data.clear();
    s_pipe_queue.clear();

    /*-------------------------------------------------
    Reset the UUID set list
    -------------------------------------------------*/
    s_uuid_set.clear();
    s_rand_gen.initialise( Chimera::millis() );
  }


  bool enqueuePipeTransfer( PipeTransfer &transfer )
  {
    using namespace Chimera::Thread;
    LockGuard lck( s_lock );

    /*-------------------------------------------------
    Ensure the pipe ID actually is registered
    -------------------------------------------------*/
    if ( ( s_uuid_set.find( transfer.pipe ) == s_uuid_set.end() ) || s_pipe_data.full() || s_pipe_queue.full() )
    {
      return false;
    }

    /*-------------------------------------------------
    Enqueue the transaction, overwriting any previously
    queued data for the request ID.
    -------------------------------------------------*/
    s_pipe_queue.push( transfer.pipe );

    PipeMap::iterator iter = s_pipe_data.find( transfer.pipe );
    if( iter == s_pipe_data.end() )
    {
      s_pipe_data.insert( { transfer.pipe, transfer } );
    }
    else
    {
      iter->second = transfer;
    }

    return true;
  }


  bool nextPipeTransfer( PipeTransfer &transfer )
  {
    using namespace Chimera::Thread;
    LockGuard lck( s_lock );

    /*-------------------------------------------------
    Ensure the queue has something for us
    -------------------------------------------------*/
    if( s_pipe_queue.empty() )
    {
      return false;
    }

    RequestId id = INVALID_REQUEST;
    s_pipe_queue.pop_into( id );

    /*-------------------------------------------------
    Pull out the data from the pipe map into the user's
    structure, then wipe the map's entry.
    -------------------------------------------------*/
    PipeMap::iterator iter = s_pipe_data.find( id );
    if( ( id == INVALID_REQUEST ) || ( iter == s_pipe_data.end() ) )
    {
      return false;
    }

    transfer = iter->second;
    iter->second = {};
    return true;
  }


  bool enqueueMemTransfer( MemTransfer &transfer )
  {
    using namespace Chimera::Thread;
    LockGuard lck( s_lock );

    /*-------------------------------------------------
    Ensure the pipe ID actually is registered
    -------------------------------------------------*/
    if ( ( s_uuid_set.find( transfer.id ) == s_uuid_set.end() ) || s_request_data.full() || s_request_queue.full() )
    {
      return false;
    }

    /*-------------------------------------------------
    Enqueue the transaction, overwriting any previously
    queued data for the request ID.
    -------------------------------------------------*/
    s_request_queue.push( transfer.id );

    MemMap::iterator iter = s_request_data.find( transfer.id );
    if( iter == s_request_data.end() )
    {
      s_request_data.insert( { transfer.id, transfer } );
    }
    else
    {
      iter->second = transfer;
    }

    return true;
  }


  bool nextMemTransfer( MemTransfer &transfer )
  {
    using namespace Chimera::Thread;
    LockGuard lck( s_lock );

    /*-------------------------------------------------
    Ensure the queue has something for us
    -------------------------------------------------*/
    if( s_request_queue.empty() )
    {
      return false;
    }

    RequestId id = INVALID_REQUEST;
    s_request_queue.pop_into( id );

    /*-------------------------------------------------
    Find the data and copy it to the user. Erase the
    mapping from the memory queue as these transfers
    aren't permanently mapped.
    -------------------------------------------------*/
    MemMap::iterator iter = s_request_data.find( id );
    if( ( id == INVALID_REQUEST ) || ( iter == s_request_data.end() ) )
    {
      return false;
    }

    transfer = iter->second;
    s_request_data.erase( iter );

    return true;
  }


}  // namespace Chimera::DMA::Util
