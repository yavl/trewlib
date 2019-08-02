#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Camera.hpp"
#include "Shader.hpp"

using namespace pm;

Texture::Texture(const char* path, Shader* shader, Camera* cam) {
	this->name = path;
	this->shader = shader;
	this->cam = cam;
	this->matrix = glm::mat4(1.0f);
	this->x = 0.0f;
	this->y = 0.0f;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	stbi_set_flip_vertically_on_load(true);
	this->image = stbi_load(path, &width, &height, 0, STBI_rgb_alpha);
	if (this->image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		fmt::print("[LOADED] Texture <{}> with size: {}x{}\n", name, width, height);
	}
	else {
        fmt::print("[ERROR] Can't load texture <{}>", name);
	}
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	float widthf = (float)width;
	float heightf = (float)height;

	float vertices[] = {
		 // positions			 // colors           // texture coords
		 widthf,  heightf, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 widthf, -heightf, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-widthf, -heightf, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-widthf,  heightf, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

Texture::~Texture() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Texture::draw(float x, float y) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	shader->use();
	matrix = glm::mat4(1.0f);
	matrix = glm::translate(matrix, glm::vec3(x, y, 0));
	shader->set_uniform("u_projTrans", cam->projection_matrix * matrix);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Texture::set_shader(Shader* shader) {
	this->shader = shader;
}
