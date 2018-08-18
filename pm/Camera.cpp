#include "Camera.hpp"

using namespace pm;

Camera::Camera() {
	matrix = glm::mat4(1.0f);
}

Camera::~Camera() {
}

void Camera::update() {
}