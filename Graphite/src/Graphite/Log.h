#pragma once

#include "Core.h"

#include "Core.h"
#include "spdlog\spdlog.h"

namespace Graphite
{
	class GRAPHITE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:

		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}


// Core Log Macros
#define GF_CORE_ERROR(...)		::Graphite::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GF_CORE_WARN(...)		::Graphite::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GF_CORE_INFO(...)		::Graphite::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GF_CORE_TRACE(...)		::Graphite::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GF_CORE_FATAL(...)		::Graphite::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client Log Macros
#define GF_CLIENT_ERROR(...)	::Graphite::Log::GetClientLogger()->error(__VA_ARGS__)
#define GF_CLIENT_WARN(...)		::Graphite::Log::GetClientLogger()->warn(__VA_ARGS__)
#define GF_CLIENT_INFO(...)		::Graphite::Log::GetClientLogger()->info(__VA_ARGS__)
#define GF_CLIENT_TRACE(...)	::Graphite::Log::GetClientLogger()->trace(__VA_ARGS__)
#define GF_CLIENT_FATAL(...)	::Graphite::Log::GetClientLogger()->fatal(__VA_ARGS__)


