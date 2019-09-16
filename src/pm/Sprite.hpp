#pragma once

namespace pm {
	class Texture;
	class Sprite {
	private:
		Texture* texture;
		float x;
		float y;
	public:
		Sprite(Texture* tex);
		virtual ~Sprite() = default;
		virtual void draw();
	};
}