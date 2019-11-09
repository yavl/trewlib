#include "Button.hpp"
#include "pm/Sprite.hpp"
#include "pm/Texture.hpp"

using namespace pm;

Button::Button(std::string name) : Widget(name) {
	tex = std::make_unique<Texture>("button.png");
	sprite = std::make_unique<Sprite>(tex.get());
}

Button::~Button() {}

void Button::draw() {
	sprite->draw();
}
