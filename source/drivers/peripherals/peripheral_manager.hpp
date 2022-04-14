/******************************************************************************
 *  File Name:
 *    peripheral_manager.hpp
 *
 *  Description:
 *    Helper class for managing a collection of device driver instances
 *
 *  2022 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

#pragma once
#ifndef CHIMERA_DEVICE_DRIVER_MANAGER_HPP
#define CHIMERA_DEVICE_DRIVER_MANAGER_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/source/drivers/threading/threading_extensions.hpp>
#include <cstddef>
#include <etl/intrusive_list.h>

namespace Chimera
{
  /*-----------------------------------------------------------------------------
  Classes
  -----------------------------------------------------------------------------*/
  template<typename DriverType, typename InstanceType, size_t MaxSize>
  class DeviceManager
  {
  public:
    /*-------------------------------------------------------------------------
    Forward Declarations
    -------------------------------------------------------------------------*/
    struct ListNode;

    /*-------------------------------------------------------------------------
    Aliases
    -------------------------------------------------------------------------*/
    using DevicePtr = DriverType *;
    using LinkType  = etl::bidirectional_link<0>;
    using ListType  = etl::intrusive_list<ListNode, LinkType>;

    /*-------------------------------------------------------------------------
    Structures
    -------------------------------------------------------------------------*/
    struct ListNode : public LinkType
    {
      InstanceType instance;  /**< Instance type associated with the driver */
      DriverType   driver;    /**< Driver object for the instance */
    };

    /*-------------------------------------------------------------------------
    Public API
    -------------------------------------------------------------------------*/
    /**
     * @brief Does a simple lookup for a device low level driver
     *
     * @param instance    Instance to find
     * @return DevicePtr
     */
    DevicePtr get( const InstanceType &instance )
    {
      Chimera::Thread::LockGuard _lck( mLock );

      auto obj = this->find( instance );
      if ( obj == mDeviceList.end() )
      {
        return nullptr;
      }

      return &obj->driver;
    }

    /**
     * @brief Does a simple lookup for a device low level driver
     * @note If the device isn't found, one will be created.
     *
     * @param instance    Instance to find
     * @return DevicePtr
     */
    DevicePtr getOrCreate( const InstanceType &instance )
    {
      Chimera::Thread::LockGuard _lck( mLock );

      auto obj = this->findOrCreate( instance );
      if ( obj == mDeviceList.end() )
      {
        return nullptr;
      }

      return &obj->driver;
    }

    /**
     * @brief Creates a new object of the given instance
     *
     * @param instance    Instance to create
     * @return True if creation succeeded, else false
     */
    bool create( const InstanceType &instance )
    {
      Chimera::Thread::LockGuard _lck( mLock );

      /*-----------------------------------------------------------------------
      Check for size limits
      -----------------------------------------------------------------------*/
      if ( mDeviceList.size() + 1 >= MaxSize )
      {
        return false;
      }

      /*-----------------------------------------------------------------------
      Allocate a new entry for the object being created
      -----------------------------------------------------------------------*/
      void *raw_mem = malloc( sizeof( ListNode ) );
      if ( !raw_mem )
      {
        return false;
      }

      /*-----------------------------------------------------------------------
      Construct and insert the entry into the list
      -----------------------------------------------------------------------*/
      ListNode *node = new ( raw_mem ) ListNode();
      node->instance = instance;
      mDeviceList.push_back( *node );
      return true;
    }

    /**
     * @brief Destroys an instance of the peripheral object
     *
     * @param instance    Instance to destroy
     */
    void destroy( const InstanceType &instance )
    {
      Chimera::Thread::LockGuard _lck( mLock );

      auto iter = this->find( instance );
      if ( iter != mDeviceList.end() )
      {
        mDeviceList.erase( iter );

        /*---------------------------------------------------------------------
        Destroy the memory, leaving no trace of the configured driver
        ---------------------------------------------------------------------*/
        void *const addr = &iter;
        memset( addr, 0xA5, sizeof( ListNode ) );
        free( addr );
      }
    }

    /**
     * @brief Gets a list of instances registered with the manager
     *
     * @param arr     Output array to fill
     * @param len     Number of elements in the array
     * @return size_t Number of elements written
     */
    size_t registeredInstances( InstanceType *const arr, const size_t len )
    {
      Chimera::Thread::LockGuard _lck( mLock );

      /*-----------------------------------------------------------------------
      Check there are enough elements in the array to fill
      -----------------------------------------------------------------------*/
      if( len < mDeviceList.size() )
      {
        return 0;
      }

      /*-----------------------------------------------------------------------
      Fill the array with registered indices
      -----------------------------------------------------------------------*/
      size_t count = 0;
      for ( auto iter = mDeviceList.begin(); iter != mDeviceList.end(); iter++ )
      {
        arr[ count ] = iter->instance;
        count++;
      }

      return count;
    }

    /**
     * @brief Gets the maximum allowed number of drivers in this structure
     * @return constexpr size_t
     */
    constexpr size_t maxSize()
    {
      return MaxSize;
    }

  private:
    Chimera::Thread::RecursiveMutex mLock;       /**< Threading lock */
    ListType                        mDeviceList; /**< Managed linked list object */

    /**
     * @brief Finds the entry object associated with the given instance
     *
     * @param instance    Which instance to search for
     * @return ListType::iterator
     */
    auto find( const InstanceType &instance )
    {
      for ( auto iter = mDeviceList.begin(); iter != mDeviceList.end(); iter++ )
      {
        if ( iter->instance == instance )
        {
          return iter;
        }
      }

      return mDeviceList.end();
    }

    /**
     * @brief Looks up the instance object, or tries to create it.
     *
     * @param instance    Which instance to search/create
     * @return ListType::iterator
     */
    auto findOrCreate( const InstanceType &instance )
    {
      auto obj = this->find( instance );

      if ( obj == mDeviceList.end() )
      {
        if ( this->create( instance ) )
        {
          obj = this->find( instance );
        }
        else
        {
          obj = mDeviceList.end();
        }
      }

      return obj;
    }
  };
}  // namespace Chimera

#endif /* !CHIMERA_DEVICE_DRIVER_MANAGER_HPP */
