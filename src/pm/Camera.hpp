#pragma once

#include "pm.hpp"
#include "Coord.hpp"

namespace pm {
	class WindowManager;
	class Camera {
	private:
		WindowManager* window;
		float camSpeed;
		float zoomFactor;
		Coord pos;
		Coord dragNew, dragOld;
		int oldState;
	public:
		glm::mat4 projection_matrix;
		static float zoom;

		Camera(WindowManager* window, int width, int height);
		virtual ~Camera() = default;
		virtual void update(float dt);
		virtual void setPosition(float x, float y);
		virtual void translate(float x, float y);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	};
}