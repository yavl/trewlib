#pragma once

namespace pm {
	class Texture;
	class Sprite {
	public:
		Sprite(Texture* tex);
		virtual ~Sprite() = default;
		virtual void draw();
	private:
		Texture* texture;
		float x;
		float y;
	};
}