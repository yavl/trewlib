#include "Sprite.hpp"
#include <trew/drawables/Drawable.hpp>

using namespace trew;

Sprite::Sprite(Drawable* drawable) : Node("Sprite") {
	this->drawable = drawable;
	float width = static_cast<float>(drawable->getImageWidth());
	float height = static_cast<float>(drawable->getImageHeight());
	setSize(width, height);
}

void Sprite::draw() const {
    // todo draw via spritebatch instead
	if (getParent())
		drawable->draw(getX() + getParent()->getX(), getY() + getParent()->getY(), getWidth(), getHeight(), color);
	else
		drawable->draw(getX(), getY(), getWidth(), getHeight(), color);
	Node::draw();
}

Color Sprite::getColor() const {
	return color;
}

void Sprite::setColor(Color color) {
	this->color = color;
}