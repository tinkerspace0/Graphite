#pragma once

#include "Graphite/Core/Base.h"
#include "Graphite/Core/Log.h"
#include <filesystem>

#ifdef GF_ENABLE_ASSERTS

// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define GF_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { GF##type##ERROR(msg, __VA_ARGS__); GF_DEBUGBREAK(); } }
#define GF_INTERNAL_ASSERT_WITH_MSG(type, check, ...) GF_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define GF_INTERNAL_ASSERT_NO_MSG(type, check) GF_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", GF_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define GF_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define GF_INTERNAL_ASSERT_GET_MACRO(...) GF_EXPAND_MACRO( GF_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, GF_INTERNAL_ASSERT_WITH_MSG, GF_INTERNAL_ASSERT_NO_MSG) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define GF_ASSERT(...) GF_EXPAND_MACRO( GF_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define GF_CORE_ASSERT(...) GF_EXPAND_MACRO( GF_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define GF_ASSERT(...)
#define GF_CORE_ASSERT(...)
#endif