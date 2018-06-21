#pragma once
#ifndef CHIMERA_LOGGING_HPP
#define CHIMERA_LOGGING_HPP

#include <Chimera/lib/spdlog/include/spdlog/spdlog.h>
#include <Chimera/lib/spdlog/include/spdlog/sinks/sink.h>


class my_sink : public spdlog::sinks::sink
{
	void log(const spdlog::details::log_msg& msg) override
	{
		// Your code here. 
		// details::log_msg is a struct containing the log entry info like level, timestamp, thread id etc.
		// msg.formatted contains the formatted log.
		// msg.raw contains pre formatted log
		//std::cout << msg.formatted.str();
	}

	void flush()
	{
		//std::cout << std::flush;
	}

};

#endif