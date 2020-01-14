#pragma once

#include "Widget.hpp"
#include <memory>

namespace trew {
	class Sprite;
	class Texture;
	class Button : public Widget {
	public:
		Button(std::string name);
		virtual ~Button();
		void draw() override;
	private:
		std::unique_ptr<Texture> tex;
		std::unique_ptr<Sprite> sprite;
	};
}
