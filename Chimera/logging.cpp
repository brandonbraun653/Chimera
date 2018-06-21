

/* Chimera Includes */
#include <Chimera/logging.hpp>

namespace Chimera
{
	namespace Logging
	{
		SimpleLogger Console;

		void SimpleLogger::applyLogLevelColoring(Level level)
		{
			switch (level)
			{
			case Level::INFO:
				printf(Colors::BRIGHT_GREEN);
				break;

			case Level::WARN:
				printf(Colors::BRIGHT_YELLOW);
				break;
				
			case Level::ERROR:
				printf(Colors::BRIGHT_MAGENTA);
				break;

			case Level::FATAL:
				printf(Colors::BRIGHT_RED);
				break;
				
			case Level::DBG:
				printf(Colors::BRIGHT_BLUE);
				break;

			default: 
				printf(Colors::RESET);
				break;
			}
		}

		void SimpleLogger::applyLogTag(Level level)
		{
			switch (level)
			{
			case Level::INFO:
				printf("[INFO]\t");
				break;

			case Level::WARN:
				printf("[WARN]\t");
				break;

			case Level::ERROR:
				printf("[ERROR]\t");
				break;

			case Level::FATAL:
				printf("[FATAL]\t");
				break;

			case Level::DBG:
				printf("[DEBUG]\t");
				break;

			default:
				printf("[]\t");
				break;
			}
		}
	}
}
