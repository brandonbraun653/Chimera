#pragma once
#ifndef CHIMERA_LOGGING_HPP
#define CHIMERA_LOGGING_HPP

/* C/C++ Includes */
#include <memory>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

/* Chimera Includes */
#include <Chimera/serial.hpp>

namespace Chimera
{
  namespace Logging
  {
    struct Colors
    {
      static constexpr const char *RESET   = "\x1B[0m";
      static constexpr const char *BLACK   = "\x1B[30m";
      static constexpr const char *RED     = "\x1B[31m";
      static constexpr const char *GREEN   = "\x1B[32m";
      static constexpr const char *YELLOW  = "\x1B[33m";
      static constexpr const char *BLUE    = "\x1B[34m";
      static constexpr const char *MAGENTA = "\x1B[35m";
      static constexpr const char *CYAN    = "\x1B[36m";
      static constexpr const char *WHITE   = "\x1B[37m";

      static constexpr const char *BRIGHT_BLACK   = "\x1B[90m";
      static constexpr const char *BRIGHT_RED     = "\x1B[91m";
      static constexpr const char *BRIGHT_GREEN   = "\x1B[92m";
      static constexpr const char *BRIGHT_YELLOW  = "\x1B[93m";
      static constexpr const char *BRIGHT_BLUE    = "\x1B[94m";
      static constexpr const char *BRIGHT_MAGENTA = "\x1B[95m";
      static constexpr const char *BRIGHT_CYAN    = "\x1B[96m";
      static constexpr const char *BRIGHT_WHITE   = "\x1B[97m";
    };

    /** Available options for tagging a log message with its intended level */
    enum class Level
    {
      LVL_INFO,
      LVL_WARN,
      LVL_ERROR,
      LVL_FATAL,
      LVL_DEBUG,
      MAX_LOG_LEVELS
    };

    /** An extremely simple logging class that directly writes a console through
     *printf(). The only features are to colorize the output and include a logging
     *level tag. Other than that, it is simply just plain'ol printf().
     **/
    class SimpleLogger
    {
    public:
      template<typename... Args>
      void log( Level level, const char *format, Args const &... args )
      {
        applyLogLevelColoring( level );
        applyLogTag( level );
        printf( format, args... );
        printf( Colors::RESET );
      }

      SimpleLogger()  = default;
      ~SimpleLogger() = default;

    private:
      void applyLogLevelColoring( Level level );
      void applyLogTag( Level level );
    };

    extern SimpleLogger Console;
  }  // namespace Logging
}  // namespace Chimera

#endif