#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <memory>

namespace Razor
{
	std::shared_ptr<Logger> Log::CoreLogger;
	std::shared_ptr<Logger> Log::ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		CoreLogger = std::make_shared<Logger>("Razor");
		CoreLogger->SetLevel(LogLevel::trace);

		ClientLogger = std::make_shared<Logger>("App");
		ClientLogger->SetLevel(LogLevel::trace);
	}

	Logger::Logger(const std::string& name) {
		m_logger = spdlog::stdout_color_mt(name);
	}

	void Logger::SetLevel(LogLevel level) {
		m_logger->set_level(static_cast<spdlog::level::level_enum>(level));
	}

	void Logger::CriticalImpl(const std::string& message) {
		m_logger->critical("{}", message);
	}

	void Logger::ErrorImpl(const std::string& message) {
		m_logger->error("{}", message);
	}

	void Logger::WarnImpl(const std::string& message) {
		m_logger->warn("{}", message);
	}

	void Logger::TraceImpl(const std::string& message) {
		m_logger->trace("{}", message);
	}

	void Logger::InfoImpl(const std::string& message) {
		m_logger->info("{}", message);
	}


	
}