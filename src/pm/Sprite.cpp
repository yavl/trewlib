#include "Sprite.hpp"
#include "Texture.hpp"

using namespace pm;

Sprite::Sprite(Texture* texture) {
	this->texture = texture;
	this->x = 0.0f;
	this->y = 0.0f;
}

void Sprite::draw() {
    // todo draw via spritebatch instead
	texture->draw(x, y);
}

float Sprite::getX() const {
	return x;
}

void Sprite::setX(float x) {
	this->x = x;
}

float Sprite::getY() const {
	return y;
}

void Sprite::setY(float y) {
	this->y = y;
}

void Sprite::setXY(float x, float y) {
	this->x = x;
	this->y = y;
}
