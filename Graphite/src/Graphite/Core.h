#pragma once


#ifdef GF_PLATFORM_WINDOWS
	#ifdef GF_BUILD_DLL
		#define GRAPHITE_API __declspec(dllexport)
	#else
		#define GRAPHITE_API __declspec(dllimport)
	#endif
#else	
	#error Graphite Only Supports Windows
#endif

#ifdef GF_ENABLE_ASSERTS
	#define GF_ASSERT(x, ...) { if(!(x)) { GF_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();} }
	#define GF_CORE_ASSERT(x, ...) { if(!(x)) { GF_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();} }
#else
	#define GF_ASSERT(x, ...)
	#define GF_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)
