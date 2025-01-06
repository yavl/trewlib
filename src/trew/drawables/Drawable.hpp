#pragma once

namespace trew {
	class Color;
	class SpriteBatch;
	class Drawable {
	public:
		virtual void draw(float x, float y, float width, float heigh, float rotation, Color color) = 0;
		virtual void draw(float x, float y, float width, float height, Color color, SpriteBatch batch) = 0;
		virtual int getImageWidth() const = 0;
		virtual int getImageHeight() const = 0;
	};
}