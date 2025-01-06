#include "Shader.hpp"
#include <trew/FileHandle.hpp>
#include <trew/Logger.hpp>
#include <trew/Color.hpp>

using namespace trew;

#define LOGTAG "Shader"

Shader::Shader(std::string vertShaderSource, std::string fragShaderSource) {
	this->vertShaderSource = vertShaderSource;
	this->fragShaderSource = fragShaderSource;
}

void Shader::compile() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
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

GLuint trew::Shader::compileShader(const char* shaderSource, Type shaderType) {
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
		char* message = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(shaderId, length, &length, message);
		logError(LOGTAG, fmt::format("Error: failed to compile shader of type: {}", (shaderType ==
			Type::VERTEX ? "vertex" : "fragment")));
		fmt::println("{}", message);
		glDeleteShader(shaderId);
		return -1;
	}
	return shaderId;
}