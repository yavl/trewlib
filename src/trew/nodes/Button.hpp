#pragma once

#include "Widget.hpp"
#include <memory>
#include <vector>
#include <functional>

namespace trew {
	class Sprite;
	class Texture;
	class Button : public Widget {
	public:
		Button(std::string name);
		virtual ~Button();
		void draw() const override;
		void onClick() override;
		void addOnClickCallback(std::function<void()>&& clickCb);
	private:
		std::unique_ptr<Texture> tex;
		std::unique_ptr<Sprite> sprite;
		std::vector<std::function<void()>> clickCallbacks;
	};
}
