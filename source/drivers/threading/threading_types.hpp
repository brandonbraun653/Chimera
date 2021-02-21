/********************************************************************************
 *  File Name:
 *    threading_types.hpp
 *
 *  Description:
 *    Types used in Chimera Threading
 *
 *  2020-2021 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_THREADING_COMMON_TYPES_HPP
#define CHIMERA_THREADING_COMMON_TYPES_HPP

/* STL Includes */
#include <cstddef>
#include <limits>

/* ETL Includes */
#include <etl/delegate.h>
#include <etl/cstring.h>

/* Chimera Includes */
#include <Chimera/assert>
#include <Chimera/common>

namespace Chimera::Thread
{
  /*-------------------------------------------------------------------------------
  Internal Namespace
  -------------------------------------------------------------------------------*/
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
      Chimera::insert_debug_breakpoint();
      while ( 1 )
      {
        continue;
      }
    }
  }  // namespace Internal

  /*-------------------------------------------------------------------------------
  Forward Declarations
  -------------------------------------------------------------------------------*/
  class Task;


  /*-------------------------------------------------------------------------------
  Aliases
  -------------------------------------------------------------------------------*/
  using TaskArg      = void *;
  using TaskFuncPtr  = void ( * )( TaskArg );
  using TaskId       = size_t;
  using TaskMsg      = uint32_t;
  using TaskDelegate = etl::delegate<void( void * )>;


  /*-------------------------------------------------------------------------------
  Constants
  -------------------------------------------------------------------------------*/
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

  static constexpr size_t MAX_NAME_LEN             = 16;
  static constexpr size_t MAX_REGISTERABLE_THREADS = 10;
  static constexpr TaskId THREAD_ID_INVALID        = 0xCCCCCCCC;


  /*-------------------------------------------------------------------------------
  Enumerations
  -------------------------------------------------------------------------------*/
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
   *  Thread execution priority levels
   */
  enum class Priority
  {
    LEVEL_0, /**< Lowest priority, indicating a task that doesn't care if it runs */
    LEVEL_1,
    LEVEL_2,
    LEVEL_3,
    LEVEL_4,
    LEVEL_5, /**< Highest priority, must "always" run when requested */

    CRITICAL  = LEVEL_5,
    IMPORTANT = LEVEL_3,
    NORMAL    = LEVEL_2,
    LOW       = LEVEL_0,

    MINIMUM = LEVEL_0,
    MAXIMUM = LEVEL_5
  };

  /**
   *  Common inter-thread communication message identifiers.
   *
   *  @note Don't change to enum class type as these values should
   *        directly compare against TaskMsg types.
   */
  enum ITCMsg : TaskMsg
  {
    TSK_MSG_NOP = 0,     /**< Base message that means do nothing */
    TSK_MSG_WAKEUP,      /**< Wake up the thread to do some processing */
    TSK_MSG_EXIT,        /**< Instructs the task to exit */
    TSK_MSG_ISR_HANDLER, /**< Execute the ISR handler */

    TSK_MSG_USR_START = 100 /**< Begin of IDs for user messages */
  };


  /*-------------------------------------------------------------------------------
  Structures
  -------------------------------------------------------------------------------*/
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


    UserFunction() : type( FunctorType::UNKNOWN ), callable( {} )
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
    Priority priority;                       /**< Tells the scheduler where this thread fits in the priority hierarchy */
    size_t stackWords;                       /**< How many bytes to allocate from the heap for this thread's stack */
    etl::string<MAX_NAME_LEN> name;          /**< User friendly name for identification */

    CommonTaskCfg() : function( {} ), arg( nullptr ), priority( Priority::LOW ), stackWords( 0 )
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
      memset( &specialization, 0, sizeof( specialization ) );
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
