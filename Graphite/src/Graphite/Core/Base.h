#pragma once

#include "Graphite/Core/PlatformDetection.h"

#include <memory>

#ifdef GF_DEBUG
#if defined(GF_PLATFORM_WINDOWS)
#define GF_DEBUGBREAK() __debugbreak()
#elif defined(GF_PLATFORM_LINUX)
#include <signal.h>
#define GF_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define GF_ENABLE_ASSERTS
#else
#define GF_DEBUGBREAK()
#endif

#define GF_EXPAND_MACRO(x) x
#define GF_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define GF_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Graphite {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}

#include "Graphite/Core/Log.h"
#include "Graphite/Core/Assert.h"