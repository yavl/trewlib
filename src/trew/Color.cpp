#include "Color.hpp"
#include <mixbox/mixbox.h>
#include <fmt/core.h>

using namespace trew;

Color::Color() : r(1.f), g(1.f), b(1.f), a(1.f) {}

Color::Color(float r, float g, float b, float a): r(r), g(g), b(b), a(a) {}

Color Color::operator+(const Color& other) const {
	unsigned char r1 = this->r * 255, g1 = this->g * 255, b1 = this->b * 255;
	unsigned char r2 = other.r * 255, g2 = other.g * 255, b2 = other.b * 255;
	float t = 0.5;
	unsigned char resultR, resultG, resultB;

	mixbox_lerp(r1, g1, b1,  // first color
		r2, g2, b2,  // second color
		t,           // mixing ratio
		&resultR, &resultG, &resultB); // result

	float red = static_cast<float>(resultR) / 255;
	float green = static_cast<float>(resultG) / 255;
	float blue = static_cast<float>(resultB) / 255;
	return Color(red, green, blue, 1.f);
}

bool Color::operator==(const Color& other) const {
	if (r == other.r && g == other.g && b == other.b)
		return true;
	return false;
}

bool Color::operator!=(const Color& b) const {
	return !(*this == b);
}

void trew::Color::operator=(const Color& other) {
	r = other.r;
	g = other.g;
	b = other.b;
	a = other.a;
}
