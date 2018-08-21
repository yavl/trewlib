#include "FileManager.hpp"

FileManager::FileManager(const char* path) {
	fin.open(path);
	if (!fin)
		fmt::print("[ERROR] Could not read file: {}", path);
}

FileManager::~FileManager() {
}

std::string FileManager::as_string() {
	std::stringstream sstr;
	sstr << fin.rdbuf();
	return sstr.str();
}