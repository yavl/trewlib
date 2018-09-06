#include "Coord.hpp"
#include "pm.hpp"
Coord::Coord(int x, int y) {
	this->x = x;
	this->y = y;
}

Coord::Coord() {}

Coord::~Coord() {}

bool Coord::equals(Coord coord) {
	if (x == coord.x && y == coord.y)
		return true;
	else return false;
}