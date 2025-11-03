#pragma once

namespace trew {
	struct Vector2 {
	public:
		Vector2();
        Vector2(float x, float y);
		virtual ~Vector2() = default;
		virtual Vector2 operator+(const Vector2& b) const;
		virtual Vector2 operator-(const Vector2& b) const;
		virtual Vector2 operator*(float scalar) const;
		virtual void operator*=(float scalar);
		virtual bool operator==(const Vector2& b) const;
		virtual bool operator!=(const Vector2& b) const;
		virtual Vector2 normalized() const;
		virtual float length() const;
		virtual float distance(const Vector2& b) const;
        
        float x;
        float y;
	private:
	};
}
