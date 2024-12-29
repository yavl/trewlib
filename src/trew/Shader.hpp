#pragma once

#include <trew/trew.hpp>
#include <trew/Asset.hpp>
#include <string>

namespace trew {
	class Color;
	class Shader : public Asset {
		enum Type {
			VERTEX, FRAGMENT
		};
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
		virtual void setUniform(const char* uniformName, Color color);
	private:
		GLuint shaderProgram;
		std::string vertShaderSource;
		std::string fragShaderSource;
		std::string vertPath;
		std::string fragPath;
		virtual GLuint compileShader(const char* shaderSource, Type shaderType);
	};
}