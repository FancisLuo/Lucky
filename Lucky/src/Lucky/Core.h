#pragma once

#ifdef LK_PLATFORM_WINDOWS

	#ifdef LK_BUILD_DLL
		#define LUCKY_API __declspec(dllexport)
	#else
		#define LUCKY_API __declspec(dllimport)
	#endif // LK_BUILD_DLL

#else
	#error Lucky only support Windows
#endif // LK_PLATFORM_WINDOWS
