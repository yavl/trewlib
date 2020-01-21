#include "Vector2.hpp"
#include <cmath>

using namespace trew;

Vector2::Vector2() {
	x = 0.f;
	y = 0.f;
}

Vector2::Vector2(float x, float y) {
	this->x = x;
	this->y = y;
}

Vector2 Vector2::operator*(float scalar) const {
	return Vector2(x * scalar, y * scalar);
}

void Vector2::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
}

bool Vector2::operator==(const Vector2& b) const {
	if (x == b.x && y == b.y)
		return true;
	return false;
}

bool Vector2::operator!=(const Vector2& b) const {
	return !(*this == b);
}

Vector2 Vector2::normalized() const {
	float inv_length = 1.f / std::sqrt(x*x + y*y);
	return *this * inv_length;
}

float Vector2::length() const {
	return std::sqrt(x*x + y*y);
}

float Vector2::distance(const Vector2& b) const {
	float dx = b.x - x;
	float dy = b.y - y;
	return std::sqrt(dx*dx + dy*dy);
}
