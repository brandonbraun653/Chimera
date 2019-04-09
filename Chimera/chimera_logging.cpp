

/* Chimera Includes */
#include <Chimera/logging.hpp>

namespace Chimera
{
  namespace Logging
  {
    SimpleLogger Console;

    void SimpleLogger::applyLogLevelColoring( Level level )
    {
      switch ( level )
      {
        case Level::LVL_INFO:
          printf( Colors::BRIGHT_GREEN );
          break;

        case Level::LVL_WARN:
          printf( Colors::BRIGHT_YELLOW );
          break;

        case Level::LVL_ERROR:
          printf( Colors::BRIGHT_MAGENTA );
          break;

        case Level::LVL_FATAL:
          printf( Colors::BRIGHT_RED );
          break;

        case Level::LVL_DEBUG:
          printf( Colors::BRIGHT_BLUE );
          break;

        default:
          printf( Colors::RESET );
          break;
      }
    }

    void SimpleLogger::applyLogTag( Level level )
    {
      switch ( level )
      {
        case Level::LVL_INFO:
          printf( "[INFO]\t" );
          break;

        case Level::LVL_WARN:
          printf( "[WARN]\t" );
          break;

        case Level::LVL_ERROR:
          printf( "[ERROR]\t" );
          break;

        case Level::LVL_FATAL:
          printf( "[FATAL]\t" );
          break;

        case Level::LVL_DEBUG:
          printf( "[DEBUG]\t" );
          break;

        default:
          printf( "[]\t" );
          break;
      }
    }
  }  // namespace Logging
}  // namespace Chimera
