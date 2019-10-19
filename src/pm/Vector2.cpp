#include "Vector2.hpp"

using namespace pm;

Vector2::Vector2() {
	x = 0.f;
	y = 0.f;
}

Vector2::Vector2(float x, float y) {
	this->x = x;
	this->y = y;
}

bool Vector2::equals(Vector2 coord) const {
	if (x == coord.x && y == coord.y)
		return true;
	else return false;
}

bool Vector2::operator==(const Vector2& b) const {
	return this->equals(b);
}

bool Vector2::operator!=(const Vector2& b) const {
	return !this->equals(b);
}