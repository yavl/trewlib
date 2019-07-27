#include "Coord.hpp"

using namespace pm;

Coord::Coord(int x, int y) {
	this->x = x;
	this->y = y;
}

Coord::Coord() {}

Coord::~Coord() {}

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