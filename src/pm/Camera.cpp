#include "Camera.hpp"
#include "WindowManager.hpp"

using namespace pm;

Camera::Camera(WindowManager* window, int width, int height) {
	this->window = window;
	this->camSpeed = 500.0f;
	this->winWidth = (float) width;
	this->winHeight = (float) height;
	this->x = 0.0f;
	this->y = 0.0f;
	projection_matrix = glm::ortho(-winWidth * zoom, winWidth * zoom, -winHeight * zoom, winHeight * zoom, -1.0f, 1.0f);
	glfwSetScrollCallback(window->getGlfwWindow(), scroll_callback);
	oldState = GLFW_RELEASE;
}

void Camera::update(float dt) {
	auto win = window->getGlfwWindow();
	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
		y -= camSpeed * dt * zoom;
	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
		x += camSpeed * dt * zoom;
	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
		y += camSpeed * dt * zoom;
	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
		x -= camSpeed * dt * zoom;

	int state = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_MIDDLE);
	double xpos, ypos;
	glfwGetCursorPos(win, &xpos, &ypos);
	int mouseX = (int)xpos;
	int mouseY = (int)ypos;
	if (state == GLFW_PRESS && oldState == GLFW_RELEASE) {
		dragNew = Coord(mouseX, mouseY);
		dragOld = dragNew;
	}
	if (state == GLFW_PRESS) {
		dragNew = Coord(mouseX, mouseY);
		if (dragNew != dragOld) {
			this->translate((dragOld.x - dragNew.x) * zoom*2, (dragNew.y - dragOld.y) * zoom*2);
			dragOld = dragNew;
		}		
	}
	oldState = state;

	projection_matrix = glm::ortho(-winWidth * zoom, winWidth * zoom, -winHeight * zoom, winHeight * zoom, -1.0f, 1.0f);
	projection_matrix = glm::translate(projection_matrix, glm::vec3(x, y, 0));
}

void Camera::updateWindowData(int width, int height) {
	this->winWidth = (float) width;
	this->winHeight = (float) height;
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