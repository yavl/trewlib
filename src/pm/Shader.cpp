#include "Shader.hpp"
#include "FileHandle.hpp"
#include "Logger.hpp"

using namespace pm;

const std::string logTag = "Shader";

Shader::Shader(std::string vertShaderSource, std::string fragShaderSource) {
	this->vertShaderSource = vertShaderSource;
	this->fragShaderSource = fragShaderSource;
}

void Shader::compile() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		logError("Shader", "Failed to initialize GLAD\n");
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	auto vertStr = vertShaderSource.c_str();
	glShaderSource(vertex_shader, 1, &vertStr, nullptr);
	glCompileShader(vertex_shader);
	GLint success;
	char infoLog[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, nullptr, infoLog);
		logError(logTag, fmt::format("<{}> vertex shader compilation failed: {}", infoLog));
	}
	else {
		log(logTag, fmt::format("<{}> succesfully compiled", vertPath));
	}

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	auto fragStr = fragShaderSource.c_str();
	glShaderSource(fragment_shader, 1, &fragStr, nullptr);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, nullptr, infoLog);
		logError(logTag, fmt::format("<{}> fragment shader compilation failed: {}", infoLog));
	}
	else {
		log(logTag, fmt::format("<{}> succesfully compiled", fragPath));
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
		logError(logTag, fmt::format("error: glGetUniformLocation {}\n", uniformName));
	}
	assert(id != -1);
	glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(uniform_mat4));
}

void Shader::setUniform(const char* uniformName, GLint uniform_i) {
	GLuint id = glGetUniformLocation(shaderProgram, uniformName);
	if (id == -1) {
		logError(logTag, fmt::format("error: glGetUniformLocation {}\n", uniformName));
	}
	assert(id != -1);
	glUniform1i(id, uniform_i);
}

void Shader::setUniform(const char* uniformName, GLfloat uniform_fv[4]) {
	GLuint id = glGetUniformLocation(shaderProgram, uniformName);
	if (id == -1) {
		logError(logTag, fmt::format("error: glGetUniformLocation {}\n", uniformName));
	}
	assert(id != -1);
	glUniform4fv(id, 4, uniform_fv);
}