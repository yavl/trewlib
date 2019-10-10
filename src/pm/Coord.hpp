#pragma once

namespace pm {
	class Coord {
	public:
		Coord();
        Coord(float x, float y);
		virtual ~Coord() = default;
		virtual bool equals(Coord coord) const;
		virtual bool operator==(const Coord& b) const;
		virtual bool operator!=(const Coord& b) const;
        
        float x;
        float y;
	private:
	};
}
