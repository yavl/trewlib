#include "pm.hpp"

pm::Sprite::Sprite() {}

pm::Sprite::Sprite(Texture* tex) {
	this->tex = tex;
}

pm::Texture* pm::Sprite::get_texture() {
	return tex;
}

GLfloat pm::Sprite::get_pos_x() {
	return x;
}
void pm::Sprite::set_pos_x(GLfloat x) {
	this->x = x;
}

GLfloat pm::Sprite::get_pos_y() {
	return y;
}
void pm::Sprite::set_pos_y(GLfloat y) {
	this->y = y;
}

void pm::Sprite::set_pos(GLfloat x, GLfloat y) {
	this->x = x;
}

void pm::Sprite::draw() {
	tex->draw_texture(x, y, tex->get_x(), tex->get_y());
}
