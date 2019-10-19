#pragma once

#include "pm.hpp"
#include "Coord.hpp"

namespace pm {
	class WindowManager;
	class Camera {
	public:
		Camera(WindowManager* window, int width, int height);
		virtual ~Camera() = default;
		virtual void update(float dt);
		virtual void setPosition(float x, float y);
		virtual void translate(float x, float y);
        
        glm::mat4 projection;
		glm::mat4 view;
	private:
		WindowManager* window;
		float camSpeed;
		float zoomFactor;
		Coord pos;
		Coord dragNew, dragOld;
		int oldState;
		float zoom;
	};
}
