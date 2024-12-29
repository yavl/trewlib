#pragma once

namespace trew {
	class Color;
	class Drawable {
	public:
		virtual void draw(float x, float y, float width, float height, Color color) = 0;
		virtual int getImageWidth() const = 0;
		virtual int getImageHeight() const = 0;
	};
}