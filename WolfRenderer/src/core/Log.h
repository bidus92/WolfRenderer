#pragma once
//HEADER FOR LOGGING INFO. UTILIZATION OF SPDLOG LIBRARY

#include <memory> //for utilization of smart pointers (shared pointers specifically) 
#include "core/core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h" //header used to log custom types we create (like our events)

namespace WolfRenderer 
{

	class WLFR_API Log
		{
			public: 
				static void Init();

				//returns our logging for our engine  
				inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
				
				//returns our logging for our client(application) utilizing our logger 
				inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

			private:
				static std::shared_ptr<spdlog::logger> s_CoreLogger; 
				static std::shared_ptr<spdlog::logger>  s_ClientLogger;

		};

}




//NOTE: the "..." in the macro paramter represents a number of arguments; can be any number
//the macro __VA_ARGS__ refers to the errors specified in the ...
//ordered by severity of message!

//Core Logger Macros
#define WLFR_CORE_TRACE(...)     ::WolfRenderer::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define WLFR_CORE_INFO(...)      ::WolfRenderer::Log::GetCoreLogger()->info(__VA_ARGS__)
#define WLFR_CORE_WARN(...)      ::WolfRenderer::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define WLFR_CORE_ERROR(...)     ::WolfRenderer::Log::GetCoreLogger()->error(__VA_ARGS__)
#define WLFR_CORE_FATAL(...)     ::WolfRenderer::Log::GetCoreLogger()->fatal(__VA_ARGS__)


//Client Logger Macros
#define WLFR_TRACE(...)     ::WolfRenderer::Log::GetClientLogger()->trace(__VA_ARGS__)
#define WLFR_INFO(...)      ::WolfRenderer::Log::GetClientLogger()->info(__VA_ARGS__)
#define WLFR_WARN(...)      ::WolfRenderer::Log::GetClientLogger()->warn(__VA_ARGS__)
#define WLFR_ERROR(...)     ::WolfRenderer::Log::GetClientLogger()->error(__VA_ARGS__)
#define WLFR_FATAL(...)     ::WolfRenderer::Log::GetClientLogger()->fatal(__VA_ARGS__)


//If a distribution build of an engine, we redefine the client logger macros as undefined
