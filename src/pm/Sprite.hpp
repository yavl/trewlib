#pragma once

namespace pm {
	class Texture;
	class Sprite {
	public:
		Sprite(Texture* tex);
		virtual ~Sprite() = default;
		virtual void draw();
		virtual float getX() const;
		virtual void setX(float x);
		virtual float getY() const;
		virtual void setY(float y);
		virtual void setXY(float x, float y);
	private:
		Texture* texture;
		float x;
		float y;
	};
}