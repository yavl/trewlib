#include "Shader.hpp"

using namespace pm;

Shader::Shader(const char* vert_path, const char* frag_path) {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		fmt::print("Failed to initialize GLAD\n");
	}

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	FileManager vert_file(vert_path);
	std::string vert_str = vert_file.as_string();
	const char* vert = vert_str.c_str();
	glShaderSource(vertexShader, 1, &vert, nullptr);
	glCompileShader(vertexShader);
	GLint success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		fmt::print("[ERROR] ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}", infoLog);
	}
	else {
		fmt::print("[SUCCESS] <{}> succesfully compiled\n", vert_path);
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	FileManager frag_file(frag_path);
	std::string frag_str = frag_file.as_string();
	const char* frag = frag_str.c_str();
	glShaderSource(fragmentShader, 1, &frag, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		fmt::print("[ERROR] ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}", infoLog);
	}
	else {
		fmt::print("[SUCCESS] <{}> succesfully compiled\n", frag_path);
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader() {
}

GLuint Shader::get_shader_program() {
	return shaderProgram;
}