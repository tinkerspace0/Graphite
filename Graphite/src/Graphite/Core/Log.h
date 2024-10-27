#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Graphite {

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

// Core log macros
#define GF_CORE_TRACE(...)    ::Graphite::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GF_CORE_INFO(...)     ::Graphite::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GF_CORE_WARN(...)     ::Graphite::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GF_CORE_ERROR(...)    ::Graphite::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GF_CORE_CRITICAL(...) ::Graphite::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define GF_TRACE(...)         ::Graphite::Log::GetClientLogger()->trace(__VA_ARGS__)
#define GF_INFO(...)          ::Graphite::Log::GetClientLogger()->info(__VA_ARGS__)
#define GF_WARN(...)          ::Graphite::Log::GetClientLogger()->warn(__VA_ARGS__)
#define GF_ERROR(...)         ::Graphite::Log::GetClientLogger()->error(__VA_ARGS__)
#define GF_CRITICAL(...)      ::Graphite::Log::GetClientLogger()->critical(__VA_ARGS__)
