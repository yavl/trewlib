#pragma once

namespace pm {
	class Coord {
	public:
		float x;
		float y;
		Coord();
		virtual ~Coord() = default;
		Coord(float x, float y);
		virtual bool equals(Coord coord) const;
		virtual bool operator==(const Coord& b) const;
		virtual bool operator!=(const Coord& b) const;
	private:
	};
}