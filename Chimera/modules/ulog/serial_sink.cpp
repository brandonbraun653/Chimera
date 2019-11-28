/********************************************************************************
 *   File Name:
 *    serial_sink.cpp
 *
 *   Description:
 *    Implements a serial based sink for the uLogger interface.
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */


/* Serial Sink Includes */
#include <Chimera/modules/ulog/serial_sink.hpp>
#include <Chimera/modules/ulog/serial_sink_config.hpp>

/* Chimera Includes */
#include <Chimera/serial.hpp>
#include <Chimera/types/serial_types.hpp>


#if defined( CHIMERA_MODULES_ULOG_SUPPORT ) && ( CHIMERA_MODULES_ULOG_SUPPORT == 1 )

namespace Chimera::Modules::uLog
{
  SerialSink::SerialSink()
  {
    enabled = false;
    logLevel = ::uLog::LogLevelType::LOG_LEVEL_MIN;
  }

  SerialSink::~SerialSink()
  {
  }

  ::uLog::ResultType SerialSink::open()
  {
  }

  ::uLog::ResultType SerialSink::close()
  {
  }

  ::uLog::ResultType SerialSink::flush()
  {
  }

  ::uLog::ResultType SerialSink::enable()
  {
    enabled = true;
    return ::uLog::ResultType::RESULT_SUCCESS;
  }

  ::uLog::ResultType SerialSink::disable()
  {
    enabled = false;
    return ::uLog::ResultType::RESULT_SUCCESS;
  }

  ::uLog::ResultType SerialSink::setLogLevel( const ::uLog::LogLevelType level )
  {
    logLevel = level;
    return ::uLog::ResultType::RESULT_SUCCESS;
  }

  ::uLog::LogLevelType SerialSink::getLogLevel()
  {
    return logLevel;
  }

  ::uLog::ResultType SerialSink::log( const ::uLog::LogLevelType level, const void *const message, const size_t length )
  {
  }
}  // namespace Chimera::Modules::uLog

#endif /* CHIMERA_MODULES_ULOG_SUPPORT */