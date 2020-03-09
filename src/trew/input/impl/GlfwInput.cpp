#include "GlfwInput.hpp"
#include <GLFW/glfw3.h>

using namespace trew;

GlfwInput::GlfwInput(GLFWwindow* window) {
	this->window = window;
}

bool GlfwInput::isKeyPressed(Key key) const {
	if (glfwGetKey(window, keyToGlfwKey(key)) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	return false;
}

int GlfwInput::keyToGlfwKey(Key key) const {
	switch (key) {
	case Key::ESCAPE: {
		return GLFW_KEY_ESCAPE;
	}
	default:
		return 0;
	}
}