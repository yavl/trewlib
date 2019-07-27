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
		~FileManager();
		std::string as_string();
		bool is_open() const;
	};
}