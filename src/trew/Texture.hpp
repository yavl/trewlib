#pragma once

#include "pm.hpp"
#include "Asset.hpp"
#include <string>

namespace trew {
	class Camera;
	class Shader;
	class Texture : public Asset {
	public:
		Texture(std::string path);
		~Texture();
		virtual void draw(float x, float y, float width, float height);
		virtual void setShader(Shader* shader);
		virtual void setCamera(Camera* cam);
		virtual int getWidth() const;
		virtual int getHeight() const;
	private:
		unsigned char* image;
		int texWidth, texHeight;
		GLuint texture, VAO, VBO, EBO;
		Shader* shader;
		std::string name;
		Camera* cam;
		glm::mat4 matrix;
		float x, y;
	};
}
