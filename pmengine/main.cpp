#include <fmt/format.h>
#include <fmt/format.cc>
#include "pmMain.cpp"
#include <OpenGL/glu.h>

const int winWidth = 800, winHeight = 600;
Main m;
pm::Camera cam;

int main() {
	GLFWwindow* window;
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	window = glfwCreateWindow(winWidth, winHeight, "pm::engine", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	fmt::print("Framebuffer size: {} {}\n", width, height);
	
	const GLFWvidmode* vimod = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(window, (vimod->width - winWidth) / 2, (vimod->height - winHeight) / 2);
	glfwMakeContextCurrent(window);
	
	m.create();
	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS)
	{
		glViewport(0, 0, width, height);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glScalef(1.0f, -1.0f, 1.0f);
		cam.update(window);
		glOrtho(0.0, width, height, 0, -1, 1.0);
		glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
		m.render();
		int state = glfwGetKey(window, GLFW_KEY_E);
		if (state == GLFW_PRESS)
			fmt::print("хуй");
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}

