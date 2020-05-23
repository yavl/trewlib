#include "Button.hpp"
#include <trew/nodes/Sprite.hpp>
#include <trew/drawables/impl_glfw/Texture.hpp>

using namespace trew;

Button::Button(std::string name) : Widget(name) {
	tex = std::make_unique<Texture>("button.png");
	sprite = std::make_unique<Sprite>(tex.get());
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
