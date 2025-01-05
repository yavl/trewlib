#pragma once

namespace trew {
	class Sprite;
	class SpriteBatch {
	public:
		SpriteBatch();
		virtual void draw(Sprite sprite) const;
	};
}