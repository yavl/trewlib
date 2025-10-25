#include "Button.hpp"
#include <trew/nodes/Sprite.hpp>
#include <trew/drawables/Drawable.hpp>

using namespace trew;

Button::Button(std::string name, Drawable* drawable) : Widget(name) {
	sprite = std::make_unique<Sprite>(drawable);
}

Button::~Button() {}

void Button::draw() const {
	sprite->draw();
}

void Button::onClick() {
	for (auto& clickCb : clickCallbacks) {
		clickCb();
	}
}

void Button::addOnClickCallback(std::function<void()>&& clickCb) {
	clickCallbacks.emplace_back(clickCb);
}
