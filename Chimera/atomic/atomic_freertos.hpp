/********************************************************************************
*  File Name:
*    atomic_freertos.hpp
*
*  Description:
*    
*
*  2020 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/

#ifndef CHIMERA_ATOMIC_FREERTOS_HPP
#define CHIMERA_ATOMIC_FREERTOS_HPP

/* Chimera Includes */
#include <Chimera/chimera.hpp>

#if defined( USING_FREERTOS )

namespace Chimera
{
  template<typename AtomicType>
  class Atomic
  {
  public:
  private:
    AtomicType var;
  };
}

#endif /* CHIMERA_FREERTOS */
#endif /* !CHIMERA_ATOMIC_FREERTOS_HPP */
