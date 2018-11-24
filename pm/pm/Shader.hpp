#pragma once

#include "pm.hpp"

namespace pm {
	class Shader {
	private:
		GLuint shaderProgram;
	public:
		Shader(const char* vert_path, const char* frag_path);
		~Shader();
		GLuint get_shader_program();
		void use();
		void set_uniform(const char* uniform_name, glm::mat4 uniform_mat4);
		void set_uniform(const char* uniform_name, GLint uniform_i);
		void set_uniform(const char* uniform_name, GLfloat uniform_fv[4]);
	};
}