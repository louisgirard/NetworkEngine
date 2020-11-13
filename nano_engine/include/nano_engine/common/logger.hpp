#pragma once

#include <spdlog/spdlog.h>

namespace nano_engine
{
#define NANO_DEBUG(fmt, ...) Logger::Instance().Log<LogLevel::Debug>(fmt, __VA_ARGS__)
#define NANO_INFO(fmt, ...) Logger::Instance().Log<LogLevel::Info>(fmt, __VA_ARGS__)
#define NANO_WARN(fmt, ...) Logger::Instance().Log<LogLevel::Warn>(fmt, __VA_ARGS__)
#define NANO_ERR(fmt, ...) Logger::Instance().Log<LogLevel::Error>(fmt, __VA_ARGS__)

	enum class LogLevel
	{
		Debug,
		Info,
		Warn,
		Error
	};

	class Logger
	{
	public:
		static Logger& Instance()
		{
			static Logger ms_logger;
			return ms_logger;
		}

		template<LogLevel level, typename... Args>
		void Log(const std::string& format, Args&&...args)
		{
			if constexpr (level == LogLevel::Debug)
			{
				spdlog::debug(format, std::forward<Args>(args)...);
			}
			else if constexpr (level == LogLevel::Info)
			{
				spdlog::info(format, std::forward<Args>(args)...);
			}
			else if constexpr (level == LogLevel::Warn)
			{
				spdlog::warn(format, std::forward<Args>(args)...);
			}
			else if constexpr (level == LogLevel::Error)
			{
				spdlog::error(format, std::forward<Args>(args)...);
			}
		}
	private:
		Logger() {}
	};
}