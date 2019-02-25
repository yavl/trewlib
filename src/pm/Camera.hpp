#pragma once

#include "pm.hpp"
#include "Coord.hpp"

namespace pm {
	class Camera {
	private:
		GLFWwindow * window;
		float cam_speed;
		float winwidth, winheight;
		float x, y;
		Coord drag_new, drag_old;
		int old_state;
	public:
		glm::mat4 projection_matrix;
		static float zoom;

		Camera(GLFWwindow* window, int width, int height);
		Camera();
		~Camera();
		void update(float dt);
		void update_window_data(int width, int height);
		void set_position(float x, float y);
		void translate(float x, float y);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	};
}