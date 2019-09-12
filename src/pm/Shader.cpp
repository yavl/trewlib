#include "Shader.hpp"
#include "FileManager.hpp"

using namespace pm;

Shader::Shader(const char* vert_path, const char* frag_path) {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		fmt::print("Failed to initialize GLAD\n");
	}

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	FileManager vert_file(vert_path);
	std::string vert_str = vert_file.as_string();
	const char* vert = vert_str.c_str();
	glShaderSource(vertex_shader, 1, &vert, nullptr);
	glCompileShader(vertex_shader);
	GLint success;
	char infoLog[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success || !vert_file.is_open()) {
		glGetShaderInfoLog(vertex_shader, 512, nullptr, infoLog);
		fmt::print("[ERROR] vertex shader compilation failed: {}\n", infoLog);
	}
	else {
		fmt::print("[SUCCESS] <{}> succesfully compiled\n", vert_path);
	}

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	FileManager frag_file(frag_path);
	std::string frag_str = frag_file.as_string();
	const char* frag = frag_str.c_str();
	glShaderSource(fragment_shader, 1, &frag, nullptr);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success || !frag_file.is_open()) {
		glGetShaderInfoLog(fragment_shader, 512, nullptr, infoLog);
		fmt::print("[ERROR] fragment shader compilation failed: {}\n", infoLog);
	}
	else {
		fmt::print("[SUCCESS] <{}> succesfully compiled\n", frag_path);
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

void Shader::setUniform(const char* uniform_name, glm::mat4 uniform_mat4) {
	GLuint id = glGetUniformLocation(shaderProgram, uniform_name);
	if (id == -1) {
		fmt::print("Error: glGetUniformLocation {}\n", uniform_name);
	}
	assert(id != -1);
	glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(uniform_mat4));
}

void Shader::setUniform(const char* uniform_name, GLint uniform_i) {
	GLuint id = glGetUniformLocation(shaderProgram, uniform_name);
	if (id == -1) {
		fmt::print("Error: glGetUniformLocation {}\n", uniform_name);
	}
	assert(id != -1);
	glUniform1i(id, uniform_i);
}

void Shader::setUniform(const char* uniform_name, GLfloat uniform_fv[4]) {
	GLuint id = glGetUniformLocation(shaderProgram, uniform_name);
	if (id == -1) {
		fmt::print("Error: glGetUniformLocation {}\n", uniform_name);
	}
	assert(id != -1);
	glUniform4fv(id, 4, uniform_fv);
}