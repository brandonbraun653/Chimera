/******************************************************************************
 *  File Name:
 *    instance_manager.hpp
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
    using LinkType = etl::bidirectional_link<0>;
    using ListType = etl::intrusive_list<ListNode, LinkType>;

    /*-------------------------------------------------------------------------
    Structures
    -------------------------------------------------------------------------*/
    struct ListNode : public LinkType
    {
      InstanceType instance;
      DriverType driver;
    };

    /*-------------------------------------------------------------------------
    Public API
    -------------------------------------------------------------------------*/
    /**
     * @brief Does a simple lookup for a device instance
     *
     * @param instance    Instance to find
     * @return DevicePtr
     */
    DevicePtr get( const InstanceType &instance )
    {
      return nullptr;
    }

    /**
     * @brief Creates a new object of the given instance
     *
     * @param instance    Instance to create
     * @return DevicePtr
     */
    DevicePtr create( const InstanceType &instance )
    {
      // Don't check for duplication. This would cause double "get" calls
      // in the
      return nullptr;
    }

    /**
     * @brief Gets an existing driver or creates a new one
     *
     * @param instance    Instance to get
     * @return DevicePtr
     */
    DevicePtr getOrCreate( const InstanceType &instance )
    {
      if( auto ptr = this->get(); ptr )
      {
        return ptr;
      }
      else
      {
        return this->create( instance );
      }
    }


    void destroy( const InstanceType &instance )
    {

    }

    size_t size()
    {
      return mDeviceList.size();
    }

    constexpr size_t maxSize()
    {
      return MaxSize;
    }

  private:
    ListType mDeviceList; /**< Managed linked list object */
  };
}  // namespace Chimera

#endif /* !CHIMERA_DEVICE_DRIVER_MANAGER_HPP */
