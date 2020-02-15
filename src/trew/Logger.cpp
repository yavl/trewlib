#include "Logger.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

using namespace trew;

Logger::Logger(bool overwriteLogfile) : logLevel(LogLevel::LOG_DEBUG) {
	spdlog::set_level(spdlog::level::debug);
	try {
		auto async_file = spdlog::basic_logger_mt("async_file_logger", "trewlib.log", overwriteLogfile);
		spdlog::set_default_logger(async_file);
	}
	catch (spdlog::spdlog_ex& e) {
		fmt::print("{}\n", e.what());
	}
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
	spdlog::info("[{}] {}", tag, message);
}

void Logger::error(std::string tag, std::string message) {
	if (logLevel == LogLevel::LOG_NONE)
		return;
	spdlog::info("[{}] {}", tag, message);
}

void Logger::debug(std::string tag, std::string message) {
	if (logLevel == LogLevel::LOG_NONE || logLevel == LogLevel::LOG_INFO)
		return;
	spdlog::debug("[{}] {}", tag, message);
}

void Logger::setLogLevel(LogLevel logLevel) {
	this->logLevel = logLevel;
	switch (logLevel) {
	case LogLevel::LOG_DEBUG: {
		spdlog::set_level(spdlog::level::debug);
		break;
	}
	case LogLevel::LOG_ERROR: {
		spdlog::set_level(spdlog::level::err);
		break;
	}
	case LogLevel::LOG_INFO: {
		spdlog::set_level(spdlog::level::info);
		break;
	}
	case LogLevel::LOG_NONE: {
		spdlog::set_level(spdlog::level::off);
		break;
	}
	default: {
		spdlog::set_level(spdlog::level::debug);
		break;
	}
	}
}
