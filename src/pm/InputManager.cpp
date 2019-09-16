#include "InputManager.hpp"
#include "pm.hpp"
#include "WindowManager.hpp"

using namespace pm;

InputManager::InputManager(WindowManager* window) {
	this->window = window;
}

void InputManager::update() {
	if (glfwGetKey(window->getGlfwWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window->getGlfwWindow(), true);
}