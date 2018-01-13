#include <fmt/format.h>
#include <fmt/format.cc>
#include "pmMain.cpp"
#include <GL/glu.h>

const int winWidth = 800, winHeight = 600;
Main m;

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
	glfwSetWindowPos(window, (vimod->width - winWidth) / 2, (vimod->height - winHeight) / 2);
	glfwMakeContextCurrent(window);

	m.create();
GLfloat w = 600;
GLfloat camx = 0;
GLfloat camy = 0;
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
camy+=0.01f;
gluOrtho2D(camx-1.0, camx+1.0, camy-1.0,camy+1.0);
		glOrtho(0.0, winWidth, winHeight, 0, -1, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		pm::render();
 
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}

void pm::render() {
	m.render();
}