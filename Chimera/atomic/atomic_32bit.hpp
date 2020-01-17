/********************************************************************************
*  File Name:
*    atomic_32bit.hpp
*
*  Description:
*    
*
*  2020 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/

#ifndef CHIMERA_ATOMIC_32BIT_SYSTEM_HPP
#define CHIMERA_ATOMIC_32BIT_SYSTEM_HPP

/* Chimera Includes */
#include <Chimera/chimera.hpp>

namespace Chimera
{
  
  template<typename AtomicType>
  class Atomic
  {
  public:

  private:
    AtomicType var;
  };
}  // namespace Chimera

#endif  /* !CHIMERA_ATOMIC_32BIT_SYSTEM_HPP */
