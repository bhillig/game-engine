#pragma once

#include <spdlog/logger.h>

class Log
{
public:
	static bool Initialize();

	static std::shared_ptr<spdlog::logger> GetCoreLogger() { return s_coreLogger; }
	static std::shared_ptr<spdlog::logger> GetClientLogger() { return s_clientLogger; }

private:
	static std::shared_ptr<spdlog::logger> s_coreLogger;
	static std::shared_ptr<spdlog::logger> s_clientLogger;
};

// Core and Client loggers
#ifdef _DEBUG
	#define LOG_CORE_TRACE(...)				Log::GetCoreLogger()->trace(__VA_ARGS__);
	#define LOG_CORE_INFO(...)				Log::GetCoreLogger()->info(__VA_ARGS__);
	#define LOG_CORE_WARN(...)				Log::GetCoreLogger()->warn(__VA_ARGS__);
	#define LOG_CORE_ERROR(...)				Log::GetCoreLogger()->error(__VA_ARGS__);
	#define LOG_CORE_CRITICAL(...)			Log::GetCoreLogger()->critical(__VA_ARGS__);
	#define LOG_TRACE(...)					Log::GetClientLogger()->trace(__VA_ARGS__);
	#define LOG_INFO(...)		            Log::GetClientLogger()->info(__VA_ARGS__);
	#define LOG_WARN(...)		            Log::GetClientLogger()->warn(__VA_ARGS__);
	#define LOG_ERROR(...)		            Log::GetClientLogger()->error(__VA_ARGS__);
	#define LOG_CRITICAL(...)	            Log::GetClientLogger()->critical(__VA_ARGS__);
#else
	#define LOG_CORE_TRACE(...)
	#define LOG_CORE_INFO(...)
	#define LOG_CORE_WARN(...)
	#define LOG_CORE_ERROR(...)
	#define LOG_CORE_CRITICAL(...)
	#define LOG_TRACE(...)
	#define LOG_INFO(...)
	#define LOG_WARN(...)
	#define LOG_ERROR(...)
	#define LOG_CRITICAL(...)
#endif
