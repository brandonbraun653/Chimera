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
#include <array>

/* Serial Sink Includes */
#include <Chimera/modules/ulog/serial_sink.hpp>
#include <Chimera/modules/ulog/serial_sink_config.hpp>

/* Chimera Includes */
#include <Chimera/serial.hpp>
#include <Chimera/types/serial_types.hpp>

/* Boost Includes */
#include <boost/circular_buffer.hpp>


#if defined( CHIMERA_MODULES_ULOG_SUPPORT ) && ( CHIMERA_MODULES_ULOG_SUPPORT == 1 )

static Chimera::Serial::SerialClass sink;
static boost::circular_buffer<uint8_t> buffer( 256 );
static std::array<uint8_t, 256> hwBuffer;

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
    Chimera::Status_t hwResult = Chimera::CommonStatusCodes::OK;
    ::uLog::ResultType sinkResult = ::uLog::ResultType::RESULT_SUCCESS;

    buffer.clear();
    buffer.linearize();

    hwBuffer.fill( 0 );

    /*------------------------------------------------
    Initialize the hardware. Some explanation on the configuration:
      1. Interrupt mode is the mostly likely supported asynchronous transfer method. 
      2. Buffering is disabled as the sink should immediately write data
      3. 
    ------------------------------------------------*/
    Chimera::Serial::Config cfg;
    cfg.baud     = static_cast<size_t>( SerialBaud );
    cfg.flow     = SerialFlowCtrl;
    cfg.parity   = SerialParity;
    cfg.stopBits = SerialStopBits;
    cfg.width    = SerialCharWid;

    auto hwMode = Chimera::Hardware::PeripheralMode::INTERRUPT;

    hwResult |= sink.assignHW( SerialChannel, SerialPins );
    hwResult |= sink.configure( cfg );
    hwResult |= sink.begin( hwMode, hwMode );
    hwResult |= sink.enableBuffering( Chimera::Hardware::SubPeripheral::TX, &buffer, hwBuffer.data(), hwBuffer.size() );

    /*------------------------------------------------
    Mask the error code into a simple pass/fail. I don't think the sinks
    in general should support complicated return codes.
    ------------------------------------------------*/
    if ( hwResult != Chimera::CommonStatusCodes::OK )
    {
      sinkResult = ::uLog::ResultType::RESULT_FAIL;
    }

    return sinkResult;
  }

  ::uLog::ResultType SerialSink::close()
  {
    ::uLog::ResultType sinkResult = ::uLog::ResultType::RESULT_SUCCESS;

    if ( sink.end() != Chimera::CommonStatusCodes::OK )
    {
      sinkResult = ::uLog::ResultType::RESULT_FAIL;
    }

    return sinkResult;
  }

  ::uLog::ResultType SerialSink::flush()
  {
    ::uLog::ResultType sinkResult = ::uLog::ResultType::RESULT_SUCCESS;

    if ( sink.flush( Chimera::Hardware::SubPeripheral::TXRX ) != Chimera::CommonStatusCodes::OK )
    {
      sinkResult = ::uLog::ResultType::RESULT_FAIL;
    }

    return sinkResult;
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
    /*------------------------------------------------
    Make sure we can actually log the data
    ------------------------------------------------*/
    if ( level < logLevel )
    {
      return ::uLog::ResultType::RESULT_INVALID_LEVEL;
    }

    /*------------------------------------------------
    Write the data and block the current thread execution
    until the transfer is complete.
    ------------------------------------------------*/
    auto hwResult = Chimera::CommonStatusCodes::OK;
    auto ulResult = ::uLog::ResultType::RESULT_SUCCESS;

    hwResult |= sink.write( reinterpret_cast<const uint8_t *const>( message ), length, 100 );
    hwResult |= sink.await( Chimera::Event::Trigger::WRITE_COMPLETE, 100 );

    if ( hwResult != Chimera::CommonStatusCodes::OK )
    {
      ulResult = ::uLog::ResultType::RESULT_FAIL;
    }

    return ulResult;
  }
}  // namespace Chimera::Modules::uLog

#endif /* CHIMERA_MODULES_ULOG_SUPPORT */