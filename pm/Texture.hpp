#pragma once

#include "pm.hpp"

namespace pm {
	class Texture {
	private:
		unsigned char* image;
		int width, height;
		GLuint texture, VAO, VBO, EBO;
		GLuint shader_id;
		std::string name;
	public:
		Texture(const char* path, GLuint shader_id);
		~Texture();
		void draw();
	};
}