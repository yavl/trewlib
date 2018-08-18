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
	};
}