#pragma once

#include "pm.hpp"

namespace pm {
	class Camera;
	class Shader;
	class Texture {
	private:
		unsigned char* image;
		int width, height;
		GLuint texture, VAO, VBO, EBO;
		Shader* shader;
		std::string name;
		Camera* cam;
		glm::mat4 matrix;
		float x, y;
	public:
		Texture(const char* path, Shader* shader, Camera* cam);
		virtual ~Texture();
		virtual void draw(float x, float y);
		virtual void setShader(Shader* shader);
	};
}
