#pragma once

#include "pm.hpp"
#include <fstream>
#include <string>

namespace pm {
	class FileManager {
	private:
		std::ifstream fin;
	public:
		FileManager(const char* path);
		virtual ~FileManager() = default;
		virtual std::string asString();
		virtual bool isOpen() const;
	};
}