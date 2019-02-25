#pragma once

#include "Texture.hpp"

namespace pm {
	class Sprite {
	private:
		Texture* texture;
		float x, y;
	public:
		Sprite(Texture* tex);
		Sprite();
		~Sprite();
		void draw();
	};
}