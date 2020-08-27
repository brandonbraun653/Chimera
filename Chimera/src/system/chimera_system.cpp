/********************************************************************************
 *  File Name:
 *    chimera_system.cpp
 *
 *  Description:
 *    Implements high level system interfaces for Chimera
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* STL Includes */
#include <cstring>

/* Chimera Includes */
#include <Chimera/src/system/system_user.hpp>
#include <Chimera/src/system/system_intf.hpp>

namespace Chimera::System
{
  static InterruptMask s_SystemInterruptState;


  static Backend::DriverConfig s_backend_driver;


  Chimera::Status_t initialize()
  {
    memset( &s_SystemInterruptState, 0, sizeof( s_SystemInterruptState ) );

    /*------------------------------------------------
    Register the backend interface with Chimera
    ------------------------------------------------*/
    auto result = Backend::registerDriver( s_backend_driver );
    if ( result != Chimera::Status::OK )
    {
      return result;
    }

    /*------------------------------------------------
    Try and invoke the registered init sequence
    ------------------------------------------------*/
    if ( s_backend_driver.isSupported && s_backend_driver.initialize )
    {
      return s_backend_driver.initialize();
    }

    return result;
  }


  Chimera::Status_t systemStartup()
  {
    if ( s_backend_driver.isSupported && s_backend_driver.systemStartup )
    {
      return s_backend_driver.systemStartup();
    }
    else
    {
      return Chimera::Status::NOT_SUPPORTED;
    }
  }


  Chimera::System::InterruptMask disableInterrupts()
  {
    if ( s_backend_driver.isSupported && s_backend_driver.disableInterrupts )
    {
      return s_backend_driver.disableInterrupts();
    }
    else
    {
      auto temp = InterruptMask();
      temp.interrupted = false;
      return temp;
    }
  }


  void enableInterrupts( Chimera::System::InterruptMask &interruptMask )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.enableInterrupts )
    {
      s_backend_driver.enableInterrupts( interruptMask );
    }
  }


  int maxConcurrentThreads()
  {
    if ( s_backend_driver.isSupported && s_backend_driver.maxConcurrentThreads )
    {
      return s_backend_driver.maxConcurrentThreads();
    }
    else
    {
      return 0;
    }
  }


  ResetEvent getResetReason()
  {
    if ( s_backend_driver.isSupported && s_backend_driver.getResetReason )
    {
      return s_backend_driver.getResetReason();
    }
    else
    {
      return ResetEvent::NOT_SUPPORTED;
    }
  }


  void getSystemInformation( Information *&info )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.getSystemInformation )
    {
      s_backend_driver.getSystemInformation( info );
    }
  }


  namespace Version
  {
    std::string_view asString()
    {
      if ( s_backend_driver.isSupported && s_backend_driver.version_AsString )
      {
        return s_backend_driver.version_AsString();
      }
      else
      {
        return {};
      }
    }


    size_t major()
    {
      if ( s_backend_driver.isSupported && s_backend_driver.version_Major )
      {
        return s_backend_driver.version_Major();
      }
      else
      {
        return 0;
      }
    }


    size_t minor()
    {
      if ( s_backend_driver.isSupported && s_backend_driver.version_Minor )
      {
        return s_backend_driver.version_Minor();
      }
      else
      {
        return 0;
      }
    }


    size_t patch()
    {
      if ( s_backend_driver.isSupported && s_backend_driver.version_Patch )
      {
        return s_backend_driver.version_Patch();
      }
      else
      {
        return 0;
      }
    }
  }

  namespace Description
  {
    std::string_view about()
    {
      if ( s_backend_driver.isSupported && s_backend_driver.desc_About )
      {
        return s_backend_driver.desc_About();
      }
      else
      {
        return {};
      }
    }


    std::string_view backendDriverName()
    {
      if ( s_backend_driver.isSupported && s_backend_driver.desc_BackendDriverName )
      {
        return s_backend_driver.desc_BackendDriverName();
      }
      else
      {
        return {};
      }
    }


    std::string_view documentationLink()
    {
      if ( s_backend_driver.isSupported && s_backend_driver.desc_DocumentationLink )
      {
        return s_backend_driver.desc_DocumentationLink();
      }
      else
      {
        return {};
      }
    }
  }
}  // namespace Chimera::System
