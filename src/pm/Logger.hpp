#pragma once

#include <string>
#define FMT_HEADER_ONLY
#include <fmt/format.h>

namespace pm {
	enum class LogLevel {
		LOG_NONE,
		LOG_DEBUG,
		LOG_ERROR,
		LOG_INFO
	};

	/**
	 * Singleton for logging
	 * 
	 * the compiler should support C++11's magic statics
	 */
	class Logger {
	public:
		static Logger& getInstance();
		void log(std::string tag, std::string message);
		void error(std::string tag, std::string message);
		void debug(std::string tag, std::string message);
		void setLogLevel(LogLevel logLevel);
	private:
		Logger();
		LogLevel logLevel;
	};

	inline void log(std::string tag, std::string message) {
		Logger::getInstance().log(tag, message);
	}

	inline void logError(std::string tag, std::string message) {
		Logger::getInstance().error(tag, message);
	}

	inline void logDebug(std::string tag, std::string message) {
		Logger::getInstance().debug(tag, message);
	}
}