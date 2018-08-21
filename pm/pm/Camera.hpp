#pragma once

#include "pm.hpp"

namespace pm {
	class Camera {
	private:
		GLFWwindow * window;
		float cam_speed;
		float zoom;
		float winwidth, winheight;
		float x, y;
	public:
		glm::mat4 projection_matrix;

		Camera(GLFWwindow* window, int width, int height);
		Camera();
		~Camera();
		void update(float dt);
		void update_projection_matrix(int width, int height);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	};
}