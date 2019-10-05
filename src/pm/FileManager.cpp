#include "FileManager.hpp"
#include <sstream>
#include "Logger.hpp"
#include <fmt/core.h>

using namespace pm;

FileManager::FileManager(const char* path) : fin(path) {
	if (!fin)
		logError("FileManager", fmt::format("Could not read file: {}\n", path));
}

std::string FileManager::asString() {
	std::stringstream sstr;
	sstr << fin.rdbuf();
	return sstr.str();
}

bool FileManager::isOpen() const {
	return fin.is_open();
}