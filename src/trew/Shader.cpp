#include "Shader.hpp"
#include <trew/FileHandle.hpp>
#include <trew/Logger.hpp>
#include <trew/Color.hpp>
#include <SDL3/SDL.h>

using namespace trew;

#define LOGTAG "Shader"

Shader::Shader(std::string vertShaderSource, std::string fragShaderSource) {
	this->vertShaderSource = vertShaderSource;
	this->fragShaderSource = fragShaderSource;
}

void Shader::compile() {
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
		logError("Shader", "Failed to initialize GLAD\n");
	auto vertStr = vertShaderSource.c_str();
	auto fragStr = fragShaderSource.c_str();
	GLuint vertex_shader = compileShader(vertStr, Type::VERTEX);
	if (vertex_shader > 0) {
		log(LOGTAG, fmt::format("Succesfully compiled shader: {}", vertPath));
	}

	GLuint fragment_shader = compileShader(fragStr, Type::FRAGMENT);
	if (vertex_shader > 0) {
		log(LOGTAG, fmt::format("Succesfully compiled shader: {}", fragPath));
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertex_shader);
	glAttachShader(shaderProgram, fragment_shader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

GLuint Shader::getShaderProgram() {
	return shaderProgram;
}

void Shader::use() {
	glUseProgram(shaderProgram);
}

void Shader::setPathInfos(std::string vertPath, std::string fragPath) {
	this->vertPath = vertPath;
	this->fragPath = fragPath;
}

void Shader::setUniform(const char* uniformName, glm::mat4 uniform_mat4) {
	GLuint id = glGetUniformLocation(shaderProgram, uniformName);
	if (id == -1) {
		logError(LOGTAG, fmt::format("error: glGetUniformLocation {}\n", uniformName));
	}
	assert(id != -1);
	glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(uniform_mat4));
}

void Shader::setUniform(const char* uniformName, GLint uniform_i) {
	GLuint id = glGetUniformLocation(shaderProgram, uniformName);
	if (id == -1) {
		logError(LOGTAG, fmt::format("error: glGetUniformLocation {}\n", uniformName));
	}
	assert(id != -1);
	glUniform1i(id, uniform_i);
}

void Shader::setUniform(const char* uniformName, GLfloat uniform_fv[4]) {
	GLuint id = glGetUniformLocation(shaderProgram, uniformName);
	if (id == -1) {
		logError(LOGTAG, fmt::format("error: glGetUniformLocation {}\n", uniformName));
	}
	assert(id != -1);
	glUniform4fv(id, 4, uniform_fv);
}

void Shader::setUniform(const char* uniformName, Color color) {
	GLuint id = glGetUniformLocation(shaderProgram, uniformName);
	if (id == -1) {
		logError(LOGTAG, fmt::format("error: glGetUniformLocation {}\n", uniformName));
	}
	assert(id != -1);
	auto uniform_vec4 = glm::vec4(color.r, color.g, color.b, color.a);
	glUniform4fv(id, 1, glm::value_ptr(uniform_vec4));
}

GLuint Shader::compileShader(const char* shaderSource, Type shaderType) {
	GLuint shaderId;
	if (shaderType == Type::VERTEX) {
		shaderId = glCreateShader(GL_VERTEX_SHADER);
	} else {
		shaderId = glCreateShader(GL_FRAGMENT_SHADER);
	}
	glShaderSource(shaderId, 1, &shaderSource, nullptr);
	glCompileShader(shaderId);

	int result;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)malloc(length * sizeof(char));
		glGetShaderInfoLog(shaderId, length, &length, message);
		logError(LOGTAG, fmt::format("Error: failed to compile shader of type: {}", (shaderType ==
			Type::VERTEX ? "vertex" : "fragment")));
		fmt::println("{}", message);
		glDeleteShader(shaderId);
		free(message);
		return -1;
	}
	return shaderId;
}

SDL_GPUShader* LoadShader(
	SDL_GPUDevice* device,
	const char* shaderFilename,
	Uint32 samplerCount,
	Uint32 uniformBufferCount,
	Uint32 storageBufferCount,
	Uint32 storageTextureCount
) {
	// Auto-detect the shader stage from the file name for convenience
	SDL_GPUShaderStage stage;
	if (SDL_strstr(shaderFilename, ".vert"))
	{
		stage = SDL_GPU_SHADERSTAGE_VERTEX;
	}
	else if (SDL_strstr(shaderFilename, ".frag"))
	{
		stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
	}
	else
	{
		SDL_Log("Invalid shader stage!");
		return NULL;
	}

	char fullPath[256];
	SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(device);
	SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;
	const char* entrypoint;

	if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV) {
		SDL_snprintf(fullPath, sizeof(fullPath), "assets/%s.spv", shaderFilename);
		format = SDL_GPU_SHADERFORMAT_SPIRV;
		entrypoint = "main";
	}
	else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL) {
		SDL_snprintf(fullPath, sizeof(fullPath), "%sContent/Shaders/Compiled/MSL/%s.msl", BasePath, shaderFilename);
		format = SDL_GPU_SHADERFORMAT_MSL;
		entrypoint = "main0";
	}
	else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL) {
		SDL_snprintf(fullPath, sizeof(fullPath), "%sContent/Shaders/Compiled/DXIL/%s.dxil", BasePath, shaderFilename);
		format = SDL_GPU_SHADERFORMAT_DXIL;
		entrypoint = "main";
	}
	else {
		SDL_Log("%s", "Unrecognized backend shader format!");
		return NULL;
	}

	size_t codeSize;
	void* code = SDL_LoadFile(fullPath, &codeSize);
	if (code == NULL)
	{
		SDL_Log("Failed to load shader from disk! %s", fullPath);
		return NULL;
	}

	SDL_GPUShaderCreateInfo shaderInfo = {
		codeSize,
		static_cast<Uint8*>(code),
		entrypoint,
		format,
		stage,
		samplerCount,
		uniformBufferCount,
		storageBufferCount,
		storageTextureCount
	};
	SDL_GPUShader* shader = SDL_CreateGPUShader(device, &shaderInfo);
	if (shader == NULL)
	{
		SDL_Log("Failed to create shader!");
		SDL_free(code);
		return NULL;
	}

	SDL_free(code);
	return shader;
}