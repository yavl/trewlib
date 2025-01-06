#include "Sprite.hpp"
#include <trew/drawables/Drawable.hpp>
#include <trew/drawables/SpriteBatch.hpp>

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
		drawable->draw(getX() + getParent()->getX(), getY() + getParent()->getY(), getWidth(), getHeight(), getRotation(), color);
	else
		drawable->draw(getX(), getY(), getWidth(), getHeight(), getRotation(), color);
	Node::draw();
}

void trew::Sprite::draw(SpriteBatch batch) const {
}

Color Sprite::getColor() const {
	return color;
}

void Sprite::setColor(Color color) {
	this->color = color;
}