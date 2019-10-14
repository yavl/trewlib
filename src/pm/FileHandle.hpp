#pragma once

#include <fstream>
#include <string>

namespace pm {
	class FileHandle {
	public:
		FileHandle(std::string path);
		virtual ~FileHandle() = default;
		virtual std::string asString() const;
		virtual void writeString(std::string str);
	private:
		std::ifstream fin;
		std::string path;
	};
}