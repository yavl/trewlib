#include "pm.hpp"

pm::Color::Color() {
	r = 1.0f;
	g = 1.0f;
	b = 1.0f;
	a = 1.0f;
}
pm::Color::Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}
