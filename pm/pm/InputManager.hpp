#pragma once

#include "pm.hpp"

class InputManager {
private:
	GLFWwindow* window;
public:
	InputManager(GLFWwindow* window);
	InputManager();
	~InputManager();
	void update();
};