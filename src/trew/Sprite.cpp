#include "Sprite.hpp"
#include <trew/Drawable.hpp>

using namespace trew;

Sprite::Sprite(Drawable* drawable) : Node() {
	this->drawable = drawable;
	float width = static_cast<float>(drawable->getImageWidth());
	float height = static_cast<float>(drawable->getImageHeight());
	setSize(width, height);
}

void Sprite::draw() const {
    // todo draw via spritebatch instead
	if (getParent())
		drawable->draw(getX() + getParent()->getX(), getY() + getParent()->getY(), getWidth(), getHeight());
	else
		drawable->draw(getX(), getY(), getWidth(), getHeight());
	Node::draw();
}