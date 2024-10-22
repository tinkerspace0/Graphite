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