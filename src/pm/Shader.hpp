#pragma once

#include "pm.hpp"

namespace pm {
	class Shader {
	private:
		GLuint shaderProgram;
	public:
		Shader(const char* vert_path, const char* frag_path);
		virtual ~Shader() = default;
		virtual GLuint getShaderProgram();
		virtual void use();
		virtual void setUniform(const char* uniform_name, glm::mat4 uniform_mat4);
		virtual void setUniform(const char* uniform_name, GLint uniform_i);
		virtual void setUniform(const char* uniform_name, GLfloat uniform_fv[4]);
	};
}