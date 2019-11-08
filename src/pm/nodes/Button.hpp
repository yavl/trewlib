#pragma once

#include "Widget.hpp"
#include <memory>

namespace pm {
	class Sprite;
	class Button : public Widget {
	public:
		Button(std::string name);
		virtual ~Button();
		void draw() override;
	private:
		std::unique_ptr<Sprite> sprite;
	};
}
