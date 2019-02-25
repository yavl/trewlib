#pragma once

class Coord {
private:
public:
	int x, y;
	Coord(int x, int y);
	Coord();
	~Coord();
	bool equals(Coord coord);
	bool operator==(const Coord& b);
	bool operator!=(const Coord& b);
};