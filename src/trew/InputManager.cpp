#include "InputManager.hpp"
#include <trew/app/Window.hpp>
#include <trew/input/Input.hpp>

using namespace trew;

InputManager::InputManager(std::weak_ptr<Window> window) : input(window.lock()->getInput()) {
	this->window = window;
}

void InputManager::update() {
	if (input.isKeyPressed(Key::ESCAPE))
		window.lock()->close();
	//if (glfwGetKey(window->getRawGlfwWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	//	glfwSetWindowShouldClose(window->getRawGlfwWindow(), true);
}