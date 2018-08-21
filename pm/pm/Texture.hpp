#pragma once

#include "pm.hpp"
#include "Camera.hpp"
#include <SOIL/SOIL.h>

namespace pm {
	class Texture {
	private:
		unsigned char* image;
		int width, height;
		GLuint texture, VAO, VBO, EBO;
		GLuint shader_id;
		std::string name;
		Camera* cam;
		glm::mat4 matrix;
		float x, y;
	public:
		Texture(const char* path, GLuint shader_id, Camera* cam);
		~Texture();
		void draw(float x, float y);
		void set_shader_program(GLuint shader_id);
	};
}