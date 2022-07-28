#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"

#include "../Lucky.h"

NAMESPACE_LUCKY_START

class LUCKY_API LuckyLog
{
public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

private:
	static std::shared_ptr<spdlog::logger> s_CoreLogger;
	static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

NAMESPACE_LUCKY_END

// core log macros
#define LK_CORE_ERROR(...)	::Lucky::LuckyLog::GetCoreLogger()->error(__VA_ARGS__)
#define LK_CORE_WARN(...)	::Lucky::LuckyLog::GetCoreLogger()->warn(__VA_ARGS__)
#define LK_CORE_INFO(...)	::Lucky::LuckyLog::GetCoreLogger()->info(__VA_ARGS__)
#define LK_CORE_TRACE(...)	::Lucky::LuckyLog::GetCoreLogger()->trace(__VA_ARGS__)
#define LK_CORE_FATAL(...)	::Lucky::LuckyLog::GetCoreLogger()->fatal(__VA_ARGS__)


// client log macros
#define LK_ERROR(...)		::Lucky::LuckyLog::GetClientLogger()->error(__VA_ARGS__)
#define LK_WARN(...)		::Lucky::LuckyLog::GetClientLogger()->warn(__VA_ARGS__)
#define LK_INFO(...)		::Lucky::LuckyLog::GetClientLogger()->info(__VA_ARGS__)
#define LK_TRACE(...)		::Lucky::LuckyLog::GetClientLogger()->trace(__VA_ARGS__)
#define LK_FATAL(...)		::Lucky::LuckyLog::GetClientLogger()->fatal(__VA_ARGS__)