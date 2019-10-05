#include "Logger.hpp"
#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/color.h>

using namespace pm;

Logger::Logger() : logLevel(LogLevel::LOG_DEBUG) {}

Logger& Logger::getInstance() {
	static Logger instance;
	return instance;
}

void Logger::log(std::string tag, std::string message) {
	if (logLevel == LogLevel::LOG_NONE || logLevel == LogLevel::LOG_ERROR)
		return;
	std::string logTag = fmt::format("[INFO]");
	fmt::print("{:>10}", logTag);
	std::string messageTag = fmt::format("[{}]", tag);
	fmt::print("{:>10} {}\n", messageTag, message);
}

void pm::Logger::error(std::string tag, std::string message) {
	if (logLevel == LogLevel::LOG_NONE)
		return;
	std::string errorTag = fmt::format("[ERROR]");
	fmt::print("{:>10}", errorTag);
	std::string messageTag = fmt::format("[{}]", tag);
	fmt::print("{:>10} {}\n", messageTag, message);
}

void pm::Logger::debug(std::string tag, std::string message) {
	if (logLevel == LogLevel::LOG_NONE || logLevel == LogLevel::LOG_INFO)
		return;
	std::string debugTag = fmt::format("[DEBUG]");
	fmt::print("{:>10}", debugTag);
	std::string messageTag = fmt::format("[{}]", tag);
	fmt::print("{:>10} {}\n", messageTag, message);
}

void Logger::setLogLevel(LogLevel logLevel) {
	this->logLevel = logLevel;
}
