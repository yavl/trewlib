#include "Camera.hpp"
#include "WindowManager.hpp"
#include <functional>
#include "Logger.hpp"

using namespace trew;

Camera::Camera(std::shared_ptr<Window> window) :
	view(1.f)
{
	this->window = window;
	camSpeed = 500.0f;
	zoomFactor = 0.15f;
	zoom = 1.f;
	oldState = GLFW_RELEASE;
	updateProjection(window->getWidth(), window->getHeight());

	window->addScrollCallback([this](double xoffset, double yoffset) {
		if (yoffset > 0)
			zoom += zoomFactor * zoom;
		else
			zoom -= zoomFactor * zoom;
	});

	window->addMouseButtonCallback([=](int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			double x, y;
			auto win = static_cast<WindowManager*>(window.get());
			glfwGetCursorPos(win->getGlfwWindow(), &x, &y);
			auto world = screenToSpace(static_cast<float>(x), static_cast<float>(y));
			log("glfw", fmt::format("World pos: {}, {}", world.x, world.y));
		}
	});

	window->addResizeCallback([this](int width, int height) {
		updateProjection(width, height);
	});
}

void Camera::update(float dt) {
	// todo cleanup
	auto win = static_cast<WindowManager*>(window.get())->getGlfwWindow();
	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
		pos.y += camSpeed * dt;
	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
		pos.x -= camSpeed * dt;
	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
		pos.y -= camSpeed * dt;
	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
		pos.x += camSpeed * dt;

	int state = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_MIDDLE);
	double xpos, ypos;
	glfwGetCursorPos(win, &xpos, &ypos);
	float mouseX = static_cast<float>(xpos);
	float mouseY = static_cast<float>(ypos);
	if (state == GLFW_PRESS && oldState == GLFW_RELEASE) {
		dragNew = Vector2(mouseX, mouseY);
		dragOld = dragNew;
	}
	if (state == GLFW_PRESS) {
		dragNew = Vector2(mouseX, mouseY);
		if (dragNew != dragOld) {
			translate(dragOld.x - dragNew.x, dragNew.y - dragOld.y);
			dragOld = dragNew;
		}		
	}
	oldState = state;

	glm::vec3 cameraPos = glm::vec3(pos.x, pos.y, 1.f);
	glm::vec3 cameraFront = glm::vec3(0.f, 0.f, 1.f);
	glm::vec3 cameraUp = glm::vec3(0.f, -1.f, 0.f);

	view = glm::lookAt(
		cameraPos,
		cameraPos + cameraFront,
		cameraUp);
	view = glm::scale(view, glm::vec3(zoom, zoom, 1.f));

	auto asd = screenToSpace(window->getWidth() / 2.f, window->getHeight() / 2.f);
	glm::vec3 center(window->getWidth() / 2, window->getHeight() / 2, 0.f);
	//view = glm::translate(view, -center);
}

void Camera::setPosXY(float x, float y) {
	pos.x = x;
	pos.y = y;
}

void Camera::translate(float x, float y) {
	pos.x += x;
	pos.y += y;
}

float trew::Camera::getX() const {
	return pos.x;
}

float trew::Camera::getY() const {
	return pos.y;
}

Vector2 Camera::screenToSpace(float x, float y) {
	int width = window->getWidth();
	int height = window->getHeight();
	auto screenPos = glm::vec2(x, height - y);
	auto world = glm::unProject(glm::vec3(screenPos, 0), view, projection, glm::vec4(0, 0, width, height));
	return Vector2(world.x, world.y);
}

void Camera::updateProjection(int width, int height) {
	float halfWidth = static_cast<float>(width) / 2.f;
	float halfHeight = static_cast<float>(height) / 2.f;
	projection = glm::ortho(-halfWidth, halfWidth, halfHeight, -halfHeight, -1.f, 1.f);
}
