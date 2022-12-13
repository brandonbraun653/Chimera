/******************************************************************************
 *  File Name:
 *    threading_types.hpp
 *
 *  Description:
 *    Types used in Chimera Threading
 *
 *  2020-2021 | Brandon Braun | brandonbraun653@gmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_THREADING_COMMON_TYPES_HPP
#define CHIMERA_THREADING_COMMON_TYPES_HPP

/* STL Includes */
#include <cstddef>
#include <cstdint>
#include <limits>

/* ETL Includes */
#include <etl/delegate.h>
#include <etl/string.h>

/* Aurora Includes */
#include <Aurora/utility>

/* Chimera Includes */
#include <Chimera/assert>
#include <Chimera/common>

namespace Chimera::Thread
{
  /*---------------------------------------------------------------------------
  Internal Namespace
  ---------------------------------------------------------------------------*/
  namespace Internal
  {
    /**
     *  Helper function that is used to initialize the delegate member
     *  of the UserFunction structure. The ETL::delegate interface has
     *  no default constructor (what would it point to?) so one must
     *  be given.
     *
     *  @param[in]  arg     Unused
     *  @return void
     */
    static void delegateInitializer( void *arg )
    {
      // Should someone end up calling this, let them know by breaking their system
      ( void )arg;
      Chimera::insert_debug_breakpoint();
      while ( 1 )
      {
        continue;
      }
    }
  }  // namespace Internal

  /*---------------------------------------------------------------------------
  Forward Declarations
  ---------------------------------------------------------------------------*/
  class Task;


  /*---------------------------------------------------------------------------
  Aliases
  ---------------------------------------------------------------------------*/
  using TaskArg      = void *;
  using TaskFuncPtr  = void ( * )( TaskArg );
  using TaskId       = size_t;
  using TaskMsg      = uint32_t;
  using TaskDelegate = etl::delegate<void( void * )>;
  using TaskName     = etl::string<16>;
  using TaskPriority = uint32_t;


  /*---------------------------------------------------------------------------
  Constants
  ---------------------------------------------------------------------------*/
  static constexpr size_t TIMEOUT_BLOCK     = std::numeric_limits<size_t>::max();
  static constexpr size_t TIMEOUT_DONT_WAIT = 0;
  static constexpr size_t TIMEOUT_1MS       = 1;
  static constexpr size_t TIMEOUT_5MS       = 5;
  static constexpr size_t TIMEOUT_10MS      = 10;
  static constexpr size_t TIMEOUT_25MS      = 25;
  static constexpr size_t TIMEOUT_50MS      = 50;
  static constexpr size_t TIMEOUT_100MS     = 100;
  static constexpr size_t TIMEOUT_500MS     = 500;
  static constexpr size_t TIMEOUT_1S        = 1000;
  static constexpr size_t TIMEOUT_1MIN      = 60 * TIMEOUT_1S;
  static constexpr size_t TIMEOUT_1HR       = 60 * TIMEOUT_1MIN;

  static constexpr size_t MAX_NAME_LEN             = TaskName::MAX_SIZE;
  static constexpr size_t MAX_REGISTERABLE_THREADS = 32;
  static constexpr TaskId THREAD_ID_INVALID        = 0xCCCCCCCC;


  /*---------------------------------------------------------------------------
  Enumerations
  ---------------------------------------------------------------------------*/
  /**
   *  Describes the available function calling types that can be
   *  registered with the Thread class. Internal use only.
   */
  enum class FunctorType
  {
    C_STYLE,
    DELEGATE,

    NUM_OPTIONS,
    UNKNOWN
  };

  /**
   *  Describes the type of task that can be created. Internal use only.
   */
  enum class TaskInitType
  {
    DYNAMIC,
    STATIC,
    RESTRICTED,

    NUM_OPTIONS,
    UNKNOWN
  };

  /**
   *  Common inter-thread communication message identifiers.
   *
   *  @note Don't change to enum class type as these values should
   *        directly compare against TaskMsg types.
   */
  enum ITCMsg : TaskMsg
  {
    TSK_MSG_NOP            = 0,           /**< Base message that means do nothing */
    TSK_MSG_WAKEUP         = ( 1u << 0 ), /**< Wake up the thread to do some processing */
    TSK_MSG_EXIT           = ( 1u << 1 ), /**< Instructs the task to exit */
    TSK_MSG_ISR_HANDLER    = ( 1u << 2 ), /**< Execute the general ISR handler */
    TSK_MSG_ISR_DATA_READY = ( 1u << 3 ), /**< Execute ISR handler that consumes data */
    TSK_MSG_ISR_ERROR      = ( 1u << 4 ), /**< Execute ISR handler that manages errors */
  };

  ENUM_CLS_BITWISE_OPERATOR( ITCMsg, | );
  ENUM_CLS_BITWISE_OPERATOR( ITCMsg, & );


  /*---------------------------------------------------------------------------
  Structures
  ---------------------------------------------------------------------------*/
  /**
   *  Wrapper for storing the possible types of function calls
   *  that can be used to create a thread.
   */
  struct UserFunction
  {
    union _callable
    {
      TaskFuncPtr pointer;
      TaskDelegate delegate;

      /*-------------------------------------------------
      Delegate has no default constructor, so default
      initialize to a function stub.
      -------------------------------------------------*/
      _callable() : delegate( TaskDelegate::create<Internal::delegateInitializer>() )
      {
      }
    } callable;       /**< User function to be turned into a thread */
    FunctorType type; /**< What kind of function call it is */


    UserFunction() : callable( {} ), type( FunctorType::UNKNOWN )
    {
    }

    UserFunction( const TaskDelegate &func )
    {
      type              = FunctorType::DELEGATE;
      callable.delegate = func;
    }

    UserFunction( const TaskFuncPtr &func )
    {
      type             = FunctorType::C_STYLE;
      callable.pointer = func;
    }
  };

  /**
   *  Properties that are shared among all task configuration types
   */
  struct CommonTaskCfg
  {
    UserFunction function;                   /**< Function pointer defining what the thread executes */
    TaskArg arg;                             /**< Any arguments to pass to the function */
    TaskPriority priority;                   /**< Tells the scheduler where this thread fits in the priority hierarchy */
    size_t stackWords;                       /**< How many bytes to allocate from the heap for this thread's stack */
    etl::string<MAX_NAME_LEN> name;          /**< User friendly name for identification */

    CommonTaskCfg() : function( {} ), arg( nullptr ), priority( 0 ), stackWords( 0 )
    {
      name.clear();
    }
  };

  /**
   *  Arguments to create a task using dynamically allocated memory off the heap
   */
  struct DynamicTask : CommonTaskCfg
  {
    // Not much to do here
  };

  /**
   *  Special parameters that apply only to static tasks
   */
  struct _StaticTaskParams
  {
    void *stackBuffer;    /**< Buffer to use as the real stack */
    size_t stackSize;     /**< Size of the stack buffer */

    _StaticTaskParams() : stackBuffer( nullptr ), stackSize( 0 )
    {
    }
  };

  /**
   *  Arguments to create a task using statically allocated memory
   */
  struct StaticTask : CommonTaskCfg
  {
    _StaticTaskParams cfg;
  };

  /**
   *  Special parameters that apply only to static tasks
   */
  struct _RestrictedTaskParams
  {
    _RestrictedTaskParams()
    {
    }
  };

  /**
   *  Arguments to create a restricted task for running with an MPU
   */
  struct RestrictedTask : CommonTaskCfg
  {
    _RestrictedTaskParams cfg;
  };

  /**
   *  Stores all possible task configuration types. Internal use only.
   */
  struct TaskConfig : CommonTaskCfg
  {
    TaskInitType type; /**< What kind of init structure is stored */

    union _xTaskConfig
    {
      _StaticTaskParams staticTask;         /**< Statically allocated task */
      _RestrictedTaskParams restrictedTask; /**< MPU protected task */

      _xTaskConfig()
      {
      }
    } specialization;

    TaskConfig() : type( TaskInitType::UNKNOWN )
    {
    }

    TaskConfig( const DynamicTask &cfg )
    {
      function   = cfg.function;
      arg        = cfg.arg;
      priority   = cfg.priority;
      stackWords = cfg.stackWords;
      name       = cfg.name;

      type = TaskInitType::DYNAMIC;
      specialization = {};
    }

    TaskConfig( const StaticTask &cfg )
    {
      function   = cfg.function;
      arg        = cfg.arg;
      priority   = cfg.priority;
      stackWords = cfg.stackWords;
      name       = cfg.name;

      type                                  = TaskInitType::STATIC;
      specialization.staticTask.stackBuffer = cfg.cfg.stackBuffer;
      specialization.staticTask.stackSize   = cfg.cfg.stackSize;
    }

    TaskConfig( const RestrictedTask &cfg )
    {
      function   = cfg.function;
      arg        = cfg.arg;
      priority   = cfg.priority;
      stackWords = cfg.stackWords;
      name       = cfg.name;

      type = TaskInitType::RESTRICTED;
      RT_HARD_ASSERT( false ); // Need to copy over data for restricted type
    }
  };


}  // namespace Chimera::Thread

#endif /* !CHIMERA_THREADING_COMMON_TYPES_HPP */
