#include "Coord.hpp"

using namespace pm;

Coord::Coord() {
	x = 0.f;
	y = 0.f;
}

Coord::Coord(float x, float y) {
	this->x = x;
	this->y = y;
}

bool Coord::equals(Coord coord) const {
	if (x == coord.x && y == coord.y)
		return true;
	else return false;
}

bool Coord::operator==(const Coord& b) const {
	return this->equals(b);
}

bool Coord::operator!=(const Coord& b) const {
	return !this->equals(b);
}