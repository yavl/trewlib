#include "Sprite.hpp"
#include "Texture.hpp"

using namespace trew;

Sprite::Sprite(Texture* texture) : Node() {
	this->texture = texture;
	float width = static_cast<float>(texture->getWidth());
	float height = static_cast<float>(texture->getHeight());
	setSize(width, height);
}

void Sprite::draw() {
    // todo draw via spritebatch instead
	if (getParent())
		texture->draw(getX() + getParent()->getX(), getY() + getParent()->getY(), getWidth(), getHeight());
	else
		texture->draw(getX(), getY(), getWidth(), getHeight());
	Node::draw();
}