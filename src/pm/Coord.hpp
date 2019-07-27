#pragma once

namespace pm {
	class Coord {
	private:
	public:
		int x, y;
		Coord(int x, int y);
		Coord();
		~Coord();
		bool equals(Coord coord) const;
		bool operator==(const Coord& b) const;
		bool operator!=(const Coord& b) const;
	};
}