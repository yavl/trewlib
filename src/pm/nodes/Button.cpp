#include "Button.hpp"
#include "pm/Sprite.hpp"

using namespace pm;

Button::Button(std::string name) : Widget(name) {
	sprite = std::make_unique<Sprite>();
}

Button::~Button() {}

void Button::draw() {
	sprite->draw();
}
