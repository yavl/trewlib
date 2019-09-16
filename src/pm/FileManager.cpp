#include "FileManager.hpp"
#include <sstream>

using namespace pm;

FileManager::FileManager(const char* path) {
	fin.open(path);
	if (!fin)
		fmt::print("[ERROR] Could not read file: {}\n", path);
}

std::string FileManager::asString() {
	std::stringstream sstr;
	sstr << fin.rdbuf();
	return sstr.str();
}

bool FileManager::isOpen() const {
	return fin.is_open();
}