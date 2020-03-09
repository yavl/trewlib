#pragma once

#include "../Input.hpp"

struct GLFWwindow;

namespace trew {
	class GlfwInput : public Input {
	public:
		virtual ~GlfwInput() = default;
		GlfwInput(GLFWwindow* window);
		bool isKeyPressed(Key key) const override;
	private:
		int keyToGlfwKey(Key key) const;

		GLFWwindow* window;
	};
}