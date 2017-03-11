#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <stdio.h>

void render();
void drawTexture(GLfloat x, GLfloat y, GLuint texWidth, GLuint texHeight);
const int winWidth = 800, winHeight = 600;

GLuint texture;
int width, height;

int main() {
	GLFWwindow* window;
	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(800, 600, "pm::engine", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	const GLFWvidmode* vimod = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(window, (vimod->width-winWidth)/2, (vimod->height-winHeight)/2);
	glfwMakeContextCurrent(window);
	texture = SOIL_load_OGL_texture(
		"tex.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_DDS_LOAD_DIRECT | SOIL_FLAG_INVERT_Y
	);

	if (!texture) {
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	printf("Texture size: %dx%d", width, height);
	while (!glfwWindowShouldClose(window))
	{
		glViewport(0, 0, winWidth, winHeight);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glScalef(1, -1, 1);
		glOrtho(0.0, winWidth, winHeight, 0, -1, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}

void render() {
	drawTexture(0, 0, 800, 600);
}

void drawTexture(GLfloat x, GLfloat y, GLuint sizeX, GLuint sizeY) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glPushMatrix();
		glTranslatef(x, y, 0);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex2f(0.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex2f(0.0f, sizeY);
			glTexCoord2f(1.0f, 1.0f);
			glVertex2f(sizeX, sizeY);
			glTexCoord2f(1.0f, 0.0f);
			glVertex2f(sizeX, 0.0f);
		glEnd();
	glPopMatrix();
}