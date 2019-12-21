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

/* C++ Includes */
#include <cstdlib>

/* uLog Includes */
#if __has_include( <uLog/uLog.hpp>)
#include <uLog/ulog.hpp>
#include <uLog/types.hpp>
#include <uLog/sinks/sink_intf.hpp>

#define CHIMERA_MODULES_ULOG_SUPPORT 1

namespace Chimera::Modules::uLog
{
  class SerialSink : public ::uLog::SinkInterface
  {
  public:
    SerialSink();
    ~SerialSink();

    ::uLog::Result open() final override;

    ::uLog::Result close() final override;

    ::uLog::Result flush() final override;

    ::uLog::IOType getIOType() final override;

    ::uLog::Result log( const ::uLog::Level level, const void *const message, const size_t length ) final override;
  };
}  // namespace Chimera::Modules::uLog


#endif /* __has_include( <uLog/uLog.hpp> )*/

#endif /* !CHIMERA_MODULES_MICRO_LOGGER_SINK_HPP */
