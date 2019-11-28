/********************************************************************************
 *   File Name:
 *    serial_sink.hpp
 *
 *   Description:
 *    Defines the interface to a serial based sink for the uLogger system.
 *
 *   Links:
 *    https://github.com/brandonbraun653/uLog
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_MODULES_MICRO_LOGGER_SINK_HPP
#define CHIMERA_MODULES_MICRO_LOGGER_SINK_HPP

/* C++ Incldues */
#include <cstdlib>

/* uLog Includes */
#if __has_include(<uLog/uLog.hpp>)
#include <uLog/ulog.hpp>
#include <uLog/types.hpp>
#include <uLog/sinks/sink_intf.hpp>

#define CHIMERA_MODULES_ULOG_SUPPORT 1
#endif 

namespace Chimera::Modules::uLog
{
  class SerialSink : public ::uLog::SinkInterface
  {
  public:
    SerialSink();
    ~SerialSink();

    ::uLog::ResultType open() final override;

    ::uLog::ResultType close() final override;

    ::uLog::ResultType flush() final override;

    ::uLog::ResultType enable() final override;

    ::uLog::ResultType disable() final override;

    ::uLog::ResultType setLogLevel( const ::uLog::LogLevelType level ) final override;

    ::uLog::LogLevelType getLogLevel() final override;

    ::uLog::ResultType log( const ::uLog::LogLevelType level, const void *const message, const size_t length ) final override;

  private:
    bool enabled;
    ::uLog::LogLevelType logLevel;
  };

}  // namespace Chimera::Modules

#endif /* !CHIMERA_MODULES_MICRO_LOGGER_SINK_HPP */
