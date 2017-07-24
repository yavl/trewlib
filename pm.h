#include <stdio.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include "stb_image.h"

typedef struct {
	GLuint texture;
	GLint x, y;
} Texture;
void render();
void drawTexture(Texture tex, GLfloat, GLfloat, GLuint, GLuint);
Texture init_texture(const char*);