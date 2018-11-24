#include "Sprite.hpp"

using namespace pm;

Sprite::Sprite(Texture* texture) {
	this->texture = texture;
	this->x = 0.0f;
	this->y = 0.0f;
}

Sprite::Sprite() {}

Sprite::~Sprite() {}

void Sprite::draw() {
	texture->draw(x, y);
}