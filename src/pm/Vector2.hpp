#pragma once

namespace pm {
	class Vector2 {
	public:
		Vector2();
        Vector2(float x, float y);
		virtual ~Vector2() = default;
		virtual bool equals(Vector2 coord) const;
		virtual bool operator==(const Vector2& b) const;
		virtual bool operator!=(const Vector2& b) const;
        
        float x;
        float y;
	private:
	};
}
