#include "FileHandle.hpp"
#include <trew/Logger.hpp>
#include <sstream>

using namespace trew;

FileHandle::FileHandle(std::string path) :
	fin(path),
	path(path)
{
	if (!fin)
		logError("FileHandle", fmt::format("Could not read file: {}", path));
}

std::string FileHandle::asString() const {
	std::stringstream sstr;
	sstr << fin.rdbuf();
	return sstr.str();
}

void FileHandle::writeString(std::string str) {
	std::ofstream fout(path);
	fout << str;
}