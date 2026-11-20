#pragma once

#include "Core.h"
#include <format>

namespace spdlog
{
	class logger;
}

namespace Razor
{
	enum LogLevel : int {
    	trace = 0,
    	debug = 1,
    	info = 2,
    	warn = 3,
    	err = 4,
    	critical = 5,
    	off = 6,
    	n_levels
	};

	class RAZOR_API Logger
	{
	public:
		Logger(const std::string& name);
		void SetLevel(LogLevel level);
		template <typename... Args>
		void Critical(std::format_string<Args...> fmt, Args &&...args) {
			CriticalImpl(std::format(fmt, std::forward<Args>(args)...));
		}
		template <typename... Args>
		void Error(std::format_string<Args...> fmt, Args &&...args) {
			ErrorImpl(std::format(fmt, std::forward<Args>(args)...));
		}
		template <typename... Args>
		void Warn(std::format_string<Args...> fmt, Args &&...args) {
			WarnImpl(std::format(fmt, std::forward<Args>(args)...));
		}
		template <typename... Args>
		void Trace(std::format_string<Args...> fmt, Args &&...args) {
			TraceImpl(std::format(fmt, std::forward<Args>(args)...));
		}
		template <typename... Args>
		void Info(std::format_string<Args...> fmt, Args &&...args) {
			InfoImpl(std::format(fmt, std::forward<Args>(args)...));
		}
	private:
		void CriticalImpl(const std::string& message);
		void ErrorImpl(const std::string& message);
		void WarnImpl(const std::string& message);
		void TraceImpl(const std::string& message);
		void InfoImpl(const std::string& message);

		std::shared_ptr<spdlog::logger> m_logger;
	}; 

	class RAZOR_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<Logger>& GetCoreLogger() { return CoreLogger; }
		inline static std::shared_ptr<Logger>& GetClientLogger() { return ClientLogger; }
	private:
		static std::shared_ptr<Logger> CoreLogger;
		static std::shared_ptr<Logger> ClientLogger;
	};
}


#define RZ_CORE_CRITICAL(...) ::Razor::Log::GetCoreLogger()->Critical(__VA_ARGS__)
#define RZ_CORE_ERROR(...) ::Razor::Log::GetCoreLogger()->Error(__VA_ARGS__)
#define RZ_CORE_WARN(...)  ::Razor::Log::GetCoreLogger()->Warn(__VA_ARGS__)
#define RZ_CORE_TRACE(...) ::Razor::Log::GetCoreLogger()->Trace(__VA_ARGS__)
#define RZ_CORE_INFO(...)  ::Razor::Log::GetCoreLogger()->Info(__VA_ARGS__)

#define RZ_CRITICAL(...) ::Razor::Log::GetClientLogger()->Critical(__VA_ARGS__)
#define RZ_ERROR(...) ::Razor::Log::GetClientLogger()->Error(__VA_ARGS__)
#define RZ_WARN(...)  ::Razor::Log::GetClientLogger()->Warn(__VA_ARGS__)
#define RZ_TRACE(...) ::Razor::Log::GetClientLogger()->Trace(__VA_ARGS__)
#define RZ_INFO(...)  ::Razor::Log::GetClientLogger()->Info(__VA_ARGS__)
