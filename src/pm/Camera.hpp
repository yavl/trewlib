#pragma once

#include "pm.hpp"
#include "Vector2.hpp"
#include <memory>

namespace pm {
	class WindowManager;
	class Camera {
	public:
		Camera(std::shared_ptr<WindowManager> window, int width, int height);
		virtual ~Camera() = default;
		virtual void update(float dt);
		virtual void setPosXY(float x, float y);
		virtual void translate(float x, float y);
        
        glm::mat4 projection;
		glm::mat4 view;
	private:
		std::shared_ptr<WindowManager> window;
		float camSpeed;
		float zoomFactor;
		Vector2 pos;
		Vector2 dragNew, dragOld;
		int oldState;
		float zoom;
	};
}
