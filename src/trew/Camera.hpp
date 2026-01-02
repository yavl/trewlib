#pragma once

#include <trew/trew.hpp>
#include <trew/Vector2.hpp>

namespace trew {
	class Window;
	class Camera {
	public:
		Camera(Window* window);
		virtual ~Camera() = default;
		virtual void update(float dt);
		virtual void setPosXY(float x, float y);
		virtual void translate(float x, float y);
		virtual float getX() const;
		virtual float getY() const;
		virtual Vector2 screenToSpace(float x, float y);
        
        glm::mat4 projection;
		glm::mat4 view;
	private:
		virtual void updateProjection(int width, int height);

		Window* window;
		float camSpeed;
		float zoomFactor;
		Vector2 pos;
		Vector2 dragNew, dragOld;
		bool oldMousePressed;
		float zoom;
	};
}
