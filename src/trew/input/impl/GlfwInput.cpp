#include "GlfwInput.hpp"
#include <GLFW/glfw3.h>

using namespace trew;

GlfwInput::GlfwInput(GLFWwindow* window) {
	this->window = window;
}

bool GlfwInput::isKeyPressed(Key key) const {
	if (glfwGetKey(window, keyToGlfwKey(key)) == GLFW_PRESS)
		return true;
	return false;
}

int GlfwInput::keyToGlfwKey(Key key) const {
	// todo make this smarter
	switch (key) {
	case Key::ESCAPE: {
		return GLFW_KEY_ESCAPE;
		break;
	}
	case Key::W: {
		return GLFW_KEY_W;
		break;
	}
	case Key::A: {
		return GLFW_KEY_A;
		break;
	}
	case Key::S: {
		return GLFW_KEY_S;
		break;
	}
	case Key::D: {
		return GLFW_KEY_D;
		break;
	}
	default:
		return 0;
	}
}