#pragma once

#include "pm.hpp"
#include "Asset.hpp"
#include <string>

namespace pm {
	class Camera;
	class Shader;
	class Texture : public Asset {
	public:
		Texture(std::string path);
		~Texture();
		virtual void draw(float x, float y);
		virtual void setShader(Shader* shader);
		virtual void setCamera(Camera* cam);
	private:
		unsigned char* image;
		int width, height;
		GLuint texture, VAO, VBO, EBO;
		Shader* shader;
		std::string name;
		Camera* cam;
		glm::mat4 matrix;
		float x, y;
	};
}
