/********************************************************************************
 *  File Name:
 *    semaphore.hpp
 *
 *  Description:
 *    Semaphore implementation for Chimera
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#ifndef CHIMERA_THREADING_SEMAPHORE_HPP
#define CHIMERA_THREADING_SEMAPHORE_HPP

/* Chimera Includes */
#include <Chimera/interface/threading_intf.hpp>
#include <Chimera/threading/types.hpp>

namespace Chimera::Threading
{
  class BinarySemaphore : public SemaphoreInterface
  {
  public:

  private:
    // Will need to figure out how to abstract this into multiple computers....
    uint8_t threadLock;
    uint8_t semaphoreValue;
  };

  class CountingSemaphore : public SemaphoreInterface
  {
  public:

  private:
  };

}  // namespace Chimera::Threading

#endif /* !CHIMERA_THREADING_SEMAPHORE_HPP */
