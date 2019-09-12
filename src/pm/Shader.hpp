#pragma once

#include "pm.hpp"

namespace pm {
	class Shader {
	private:
		GLuint shaderProgram;
	public:
		Shader(const char* vert_path, const char* frag_path);
		~Shader() = default;
		GLuint getShaderProgram();
		void use();
		void setUniform(const char* uniform_name, glm::mat4 uniform_mat4);
		void setUniform(const char* uniform_name, GLint uniform_i);
		void setUniform(const char* uniform_name, GLfloat uniform_fv[4]);
	};
}