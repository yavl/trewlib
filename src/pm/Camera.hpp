#pragma once

#include "pm.hpp"
#include "Coord.hpp"

namespace pm {
	class WindowManager;
	class Camera {
	private:
		WindowManager* window;
		float camSpeed;
		float winWidth, winHeight;
		float x, y;
		Coord dragNew, dragOld;
		int oldState;
	public:
		glm::mat4 projection_matrix;
		static float zoom;

		Camera(WindowManager* window, int width, int height);
		~Camera() = default;
		void update(float dt);
		void updateWindowData(int width, int height);
		void set_position(float x, float y);
		void translate(float x, float y);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	};
}