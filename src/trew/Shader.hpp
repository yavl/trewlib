#pragma once

#include <trew/trew.hpp>
#include <trew/Asset.hpp>
#include <string>
#include <SDL3/SDL.h>

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

namespace trew {
SDL_GPUShader* loadShader(SDL_GPUDevice* device,
                          const char* assetsDirectory,
                          const char* shaderFilename,
                          Uint32 samplerCount,
                          Uint32 uniformBufferCount,
                          Uint32 storageBufferCount,
                          Uint32 storageTextureCount
                          );
}
