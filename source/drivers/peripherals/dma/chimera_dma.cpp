/********************************************************************************
 *  File Name:
 *    chimera_dma.cpp
 *
 *  Description:
 *    Implements Chimera DMA
 *
 *  2020-2021 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* STL Includes */
#include <memory>
#include <cstring>

/* Chimera Includes */
#include <Chimera/dma>

namespace Chimera::DMA
{
  /*-------------------------------------------------------------------------------
  Static Data
  -------------------------------------------------------------------------------*/
  static Backend::DriverConfig s_backend_driver;

  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  namespace Backend
  {
    Chimera::Status_t __attribute__( ( weak ) ) registerDriver( Chimera::DMA::Backend::DriverConfig &registry )
    {
      registry.isSupported = false;
      return Chimera::Status::NOT_SUPPORTED;
    }
  }  // namespace Backend


  Chimera::Status_t initialize()
  {
    /*-------------------------------------------------
    Initialize Chimera
    -------------------------------------------------*/
    memset( &s_backend_driver, 0, sizeof( s_backend_driver ) );
    Util::initializeQueues();

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
    else
    {
      return Chimera::Status::NOT_SUPPORTED;
    }
  }


  Chimera::Status_t reset()
  {
    if ( s_backend_driver.isSupported && s_backend_driver.reset )
    {
      return s_backend_driver.reset();
    }
    else
    {
      return Chimera::Status::NOT_SUPPORTED;
    }
  }


  RequestId constructPipe( const PipeConfig &config )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.constructPipe )
    {
      return s_backend_driver.constructPipe( config );
    }
    else
    {
      return INVALID_REQUEST;
    }
  }


  RequestId transfer( const MemTransfer &transfer )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.memTransfer )
    {
      return s_backend_driver.memTransfer( transfer );
    }
    else
    {
      return INVALID_REQUEST;
    }
  }


  RequestId transfer( const PipeTransfer &transfer )
  {
    if ( s_backend_driver.isSupported && s_backend_driver.pipeTransfer )
    {
      return s_backend_driver.pipeTransfer( transfer );
    }
    else
    {
      return INVALID_REQUEST;
    }
  }

}  // namespace Chimera::DMA
