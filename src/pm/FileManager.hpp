#pragma once

#include <fstream>
#include <string>

namespace pm {
	class FileManager {
	public:
		FileManager(const char* path);
		virtual ~FileManager() = default;
		virtual std::string asString();
		virtual bool isOpen() const;
	private:
		std::ifstream fin;
	};
}