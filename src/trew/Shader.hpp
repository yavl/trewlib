#pragma once

#include "pm.hpp"
#include "Asset.hpp"
#include <string>

namespace trew {
	class Shader : public Asset {
	public:
		Shader(std::string vertShaderSource, std::string fragShaderSource);
		virtual ~Shader() = default;
		virtual GLuint getShaderProgram();
		virtual void compile();
		virtual void use();
		virtual void setPathInfos(std::string vertPath, std::string fragPath);
		virtual void setUniform(const char* uniformName, glm::mat4 uniform_mat4);
		virtual void setUniform(const char* uniformName, GLint uniform_i);
		virtual void setUniform(const char* uniformName, GLfloat uniform_fv[4]);
	private:
		GLuint shaderProgram;
		std::string vertShaderSource;
		std::string fragShaderSource;
		std::string vertPath;
		std::string fragPath;
	};
}