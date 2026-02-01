#include <Log.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> Log::s_coreLogger;
std::shared_ptr<spdlog::logger> Log::s_clientLogger;

bool Log::Initialize()
{
	// Set Log Pattern
	spdlog::set_pattern("%^[%T] %n: %v%$");

	// Initialize Core Logger
	s_coreLogger = spdlog::stdout_color_mt("ENGINE");
	s_coreLogger->set_level(spdlog::level::trace);

	// Initialize Client Logger
	s_clientLogger = spdlog::stdout_color_mt("APP");
	s_clientLogger->set_level(spdlog::level::trace);

	return true;
}
