#include "Camera.hpp"

using namespace pm;

Camera::Camera(GLFWwindow* window, int width, int height) {
	this->window = window;
	this->cam_speed = 500.0f;
	this->winwidth = (float) width;
	this->winheight = (float) height;
	this->x = 0.0f;
	this->y = 0.0f;
	projection_matrix = glm::ortho(-winwidth * zoom, winwidth * zoom, -winheight * zoom, winheight * zoom, -1.0f, 1.0f);
	glfwSetScrollCallback(window, scroll_callback);
	old_state = GLFW_RELEASE;
}

Camera::Camera() {}

Camera::~Camera() {
}

void Camera::update(float dt) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		y -= cam_speed * dt * zoom;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		x += cam_speed * dt * zoom;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		y += cam_speed * dt * zoom;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		x -= cam_speed * dt * zoom;

	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	int mouseX = (int)xpos;
	int mouseY = (int)ypos;
	if (state == GLFW_PRESS && old_state == GLFW_RELEASE) {
		drag_new = Coord(mouseX, mouseY);
		drag_old = drag_new;
	}
	if (state == GLFW_PRESS) {
		drag_new = Coord(mouseX, mouseY);
		if (!drag_new.equals(drag_old)) {
			this->translate((drag_old.x - drag_new.x) * zoom*2, (drag_new.y - drag_old.y) * zoom*2);
			drag_old = drag_new;
		}		
	}
	old_state = state;

	projection_matrix = glm::ortho(-winwidth * zoom, winwidth * zoom, -winheight * zoom, winheight * zoom, -1.0f, 1.0f);
	projection_matrix = glm::translate(projection_matrix, glm::vec3(x, y, 0));
}

void Camera::update_window_data(int width, int height) {
	this->winwidth = (float) width;
	this->winheight = (float) height;
}

void Camera::set_position(float x, float y) {
	this->x = x;
	this->y = y;
}

void Camera::translate(float x, float y) {
	this->x -= x;
	this->y -= y;
}

void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (yoffset > 0)
		zoom -= 0.15f * zoom;
	else zoom += 0.15f * zoom;
}

float Camera::zoom = 1.0f;