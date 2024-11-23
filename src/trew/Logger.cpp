#include "Logger.hpp"
#include <fmt/core.h>

using namespace trew;

Logger::Logger(bool overwriteLogfile) : logLevel(LogLevel::LOG_DEBUG) {
}

Logger::~Logger() {
}

Logger& Logger::getInstance() {
	static Logger instance;
	return instance;
}

void Logger::log(std::string tag, std::string message) {
	if (logLevel == LogLevel::LOG_NONE || logLevel == LogLevel::LOG_ERROR)
		return;
	fmt::print("[{}] {}\n", tag, message);
}

void Logger::error(std::string tag, std::string message) {
	if (logLevel == LogLevel::LOG_NONE)
		return;
	fmt::print("[{}] {}\n", tag, message);
}

void Logger::debug(std::string tag, std::string message) {
	if (logLevel == LogLevel::LOG_NONE || logLevel == LogLevel::LOG_INFO)
		return;
	fmt::print("[{}] {}\n", tag, message);
}

void Logger::setLogLevel(LogLevel logLevel) {
	this->logLevel = logLevel;
	switch (logLevel) {
	case LogLevel::LOG_DEBUG: {
		break;
	}
	case LogLevel::LOG_ERROR: {
		break;
	}
	case LogLevel::LOG_INFO: {
		break;
	}
	case LogLevel::LOG_NONE: {
		break;
	}
	default: {
		break;
	}
	}
}
