#pragma once

#include <memory>
#include <iostream>

#ifdef RZ_DEBUG
	#if defined(RZ_PLATFORM_WINDOWS)
		#define RZ_DEBUGBREAK() __debugbreak()
	#elif defined(RZ_PLATFORM_LINUX)
		#include <signal.h>
		#define RZ_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define RZ_ENABLE_ASSERTS
#else
	#define RZ_DEBUGBREAK()
#endif

#define RZ_EXPAND_MACRO(x) x
#define RZ_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define RZ_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#ifdef RZ_PLATFORM_WINDOWS
	#ifdef RZ_BUILD_DLL
		#define RAZOR_API __declspec(dllexport)
	#else
		#define RAZOR_API __declspec(dllimport)
	#endif
#else
	#define RAZOR_API
#endif

namespace Razor {

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


