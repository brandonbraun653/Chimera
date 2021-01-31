/********************************************************************************
 *  File Name:
 *    queue.hpp
 *
 *  Description:
 *    Queue implementation for Chimera
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_THREADING_QUEUE_HPP
#define CHIMERA_THREADING_QUEUE_HPP

/* STL Includes */
#include <cstdint>
#include <cstddef>

/* Chimera Includes */
#include <Chimera/source/drivers/threading/threading_mutex.hpp>
#include <Chimera/source/drivers/threading/threading_detail.hpp>

namespace Chimera::Threading
{
  /**
   *  A generic queue implementation that tries to follow the
   *  STL interface as much as possible while acknowledging
   *  the unique environment constraints that an embedded platform
   *  presents.
   *
   *  As such, this queue is allocated statically and member access
   *  is performed via pointers. All functions are thread safe and
   *  some are even capable of use inside of ISRs.
   */
  class Queue : public Chimera::Threading::Lockable<Queue>
  {
  public:
    Queue();
    ~Queue();

    /**
     *  Creates a queue using statically allocated memory. If called after
     *  the queue has been created previously, the queue memory and internal
     *  structures will be reset.
     *
     *  @param[in]  qLen          Max items the queue can hold at any time
     *  @param[in]  itemSize      Size of each item in bytes
     *  @param[in]  qBuffer       Storage buffer of at least size (qLen * itemSize)
     */
    bool create( const size_t qLen, const size_t itemSize, void *qBuffer );

    /*-------------------------------------------------
    Capacity
    -------------------------------------------------*/
    /**
     *  Checks if the container is empty
     *  @return bool
     */
    bool empty();

    /**
     *  ISR variant to check if a container is empty
     *  @return bool
     */
    bool isrEmpty() const;

    /**
     *  Checks if the container is full
     *  @return bool
     */
    bool full();

    /**
     *  ISR variant to check if the container is full
     *  @return bool
     */
    bool isrFull() const;

    /**
     *  Number of elements currently in the container
     *  @return size_t
     */
    size_t size();

    /**
     *  ISR variant to get number of elements currently in the container
     *  @return size_t
     */
    size_t isrSize() const;

    /**
     *  Number of elements that can be pushed into the container
     *  @return size_t
     */
    size_t available();

    /**
     *  Returns maximum possible number of elements storable
     *  @return size_t
     */
    size_t max_size() const;

    /*-------------------------------------------------
    Modifiers
    -------------------------------------------------*/
    /**
     *  Pushes a single element to the back of the queue
     *
     *  @param[in]  element       The element to be pushed back
     *  @param[in]  timeout       How long to wait for access
     *  @return bool
     */
    bool push( const void *element, const size_t timeout );

    /**
     *  ISR variant to push a single element to the back of the queue
     *
     *  @param[in]  element       The element to be pushed back
     *  @return bool
     */
    bool isrPush( const void *buffer );

    /**
     *  Removes a single item from the front of the queue
     *
     *  @param[out] element       Variable to write the queue element into
     *  @param[in]  timeout       How long to wait for access
     *  @return bool
     */
    bool pop( void *element, const size_t timeout );

    /**
     *  ISR variant to remove a single item from the front of the queue
     *
     *  @param[out] element       Variable to write the queue element into
     *  @return bool
     */
    bool isrPop( void *buffer );

    /**
     *  Makes a copy of the first element in the queue, but does
     *  not remove it.
     *
     *  @param[out] element       Variable to write the queue element into
     *  @param[in]  timeout       How long to wait for access
     *  @return bool
     */
    bool peek( void *element, const size_t timeout );

    /**
     *  ISR variant to make a copy of the first element in the queue, but does
     *  not remove it.
     *
     *  @param[out] element       Variable to write the queue element into
     *  @return bool
     */
    bool isrPeek( void *buffer );

    /**
     *  Empties the queue to contain no elements.
     *
     *  @warning Do not call from an ISR!
     *
     *  @param[in]  timeout       How long to wait for access
     *  @return bool
     */
    bool clear( const size_t timeout );

    /*-------------------------------------------------
    Internal Data
    -------------------------------------------------*/
    /**
     *  Gets a handle to the underlying queue implementation
     *  @return detail::native_queue
     */
    detail::native_queue& getHandle();

  private:
    friend Chimera::Threading::Lockable<Queue>;


    size_t mMaxSize;
    detail::native_queue mQueueHandle;
    detail::native_queue_structure mQueueStructure;
  };

}  // namespace Chimera::Threading

#endif /* !CHIMERA_THREADING_QUEUE_HPP */
