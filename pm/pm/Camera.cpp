#include "Camera.hpp"
#include <iostream>

using namespace pm;

Camera::Camera(GLFWwindow* window, int width, int height) {
	this->window = window;
	this->cam_speed = 500.0f;
	this->zoom = 1.0f;
	this->winwidth = (float) width;
	this->winheight = (float) height;
	this->x = 0.0f;
	this->y = 0.0f;
	projection_matrix = glm::ortho(0.0f, winwidth * zoom, 0.0f, winheight * zoom, -1.0f, 1.0f);
	glfwSetScrollCallback(window, scroll_callback);
}

Camera::Camera() {}

Camera::~Camera() {
}

void Camera::update(float dt) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		y -= cam_speed * dt;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		x += cam_speed * dt;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		y += cam_speed * dt;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		x -= cam_speed * dt;

	projection_matrix = glm::ortho(-winwidth * zoom, winwidth * zoom, -winheight * zoom, winheight * zoom, -1.0f, 1.0f);
	projection_matrix = glm::translate(projection_matrix, glm::vec3(x, y, 0));
}

void Camera::update_projection_matrix(int width, int height) {
	this->winwidth = (float) width;
	this->winheight = (float) height;
	projection_matrix = glm::ortho(-winwidth, winwidth, -winheight, winheight, -1.0f, 1.0f);
}

void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (yoffset > 0) {
		// зум потом добавить
	}
}