#pragma once

struct GLFWwindow;

namespace pm {
	class InputManager {
	private:
		GLFWwindow* window;
	public:
		InputManager(GLFWwindow* window);
		InputManager();
		~InputManager();
		void update();
	};
}