#include "InputManager.hpp"

InputManager::InputManager(GLFWwindow* window) {
	this->window = window;
}

InputManager::InputManager() {}

InputManager::~InputManager() {
}

void InputManager::update() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}