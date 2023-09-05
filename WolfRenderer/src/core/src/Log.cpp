#include "core/Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"



namespace WolfRenderer
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		//see the spdlog wiki and pattern flags for details in format patterns :) 
		//Time, name (color coding starts, log name type, message, end color coating) 
		spdlog::set_pattern("[%T]: %^%n: ::%l:: -%v%$ ");

		s_CoreLogger = spdlog::stdout_color_mt("WOLFRENDERER");

		//trace with spdlog::level will print all log levels in effect 
		s_CoreLogger->set_level(spdlog::level::trace); 

		s_ClientLogger = spdlog::stdout_color_mt("APP");

		s_ClientLogger->set_level(spdlog::level::trace);
	}

}
