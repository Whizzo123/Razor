#pragma once

#ifdef RZ_PLATFORM_WINDOWS
#if RZ_DYNAMIC_LINK
	#ifdef RZ_BUILD_DLL
		#define RAZOR_API __declspec(dllexport)
	#else
		#define RAZOR_API __declspec(dllimport)
	#endif // DEBUG
#else
	#define RAZOR_API 
#endif
#else
	#error Razor only support Window!
#endif


