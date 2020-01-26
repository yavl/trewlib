#pragma once

namespace trew {
	class Drawable {
	public:
		virtual void draw(float x, float y, float width, float height) = 0;
		virtual int getImageWidth() const = 0;
		virtual int getImageHeight() const = 0;
	};
}