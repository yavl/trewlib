#include "FileManager.hpp"
#include <sstream>

using namespace pm;

FileManager::FileManager(const char* path) {
	fin.open(path);
	if (!fin)
		fmt::print("[ERROR] Could not read file: {}\n", path);
}

FileManager::~FileManager() {
}

std::string FileManager::as_string() {
	std::stringstream sstr;
	sstr << fin.rdbuf();
	return sstr.str();
}

bool FileManager::is_open() const {
	return fin.is_open();
}