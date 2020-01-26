#pragma once

namespace trew {
	class Action;
	class Actor {
	public:
		virtual ~Actor() = default;
		virtual void act(float dt) = 0;
		virtual void addAction(Action* action) = 0;

		virtual float getX() const = 0;
		virtual void setX(float x) = 0;
		virtual float getY() const = 0;
		virtual void setY(float y) = 0;
		virtual void setXY(float x, float y) = 0;
		virtual float getWidth() const = 0;
		virtual void setWidth(float width) = 0;
		virtual float getHeight() const = 0;
		virtual void setHeight(float height) = 0;
		virtual void setSize(float width, float height) = 0;
	};
}