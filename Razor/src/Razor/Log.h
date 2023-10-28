#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"

namespace Razor
{
	class RAZOR_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> CoreLogger;
		static std::shared_ptr<spdlog::logger> ClientLogger;

	};
}


#define RZ_CORE_CRITICAL(...) ::Razor::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define RZ_CORE_ERROR(...) ::Razor::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RZ_CORE_WARN(...)  ::Razor::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RZ_CORE_TRACE(...) ::Razor::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RZ_CORE_INFO(...)  ::Razor::Log::GetCoreLogger()->info(__VA_ARGS__)

#define RZ_CRITICAL(...) ::Razor::Log::GetClientLogger()->critical(__VA_ARGS__)
#define RZ_ERROR(...) ::Razor::Log::GetClientLogger()->error(__VA_ARGS__)
#define RZ_WARN(...)  ::Razor::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RZ_TRACE(...) ::Razor::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RZ_INFO(...)  ::Razor::Log::GetClientLogger()->info(__VA_ARGS__)
