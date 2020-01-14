#include "Camera.hpp"
#include "WindowManager.hpp"
#include <functional>
#include "Logger.hpp"

using namespace trew;

Camera::Camera(std::shared_ptr<WindowManager> window) :
	view(1.0f)
{
	this->window = window;
	camSpeed = 500.0f;
	zoomFactor = 0.15f;
	zoom = 1.0f;
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
			glfwGetCursorPos(window->getGlfwWindow(), &x, &y);
			int width = window->getWidth();
			int height = window->getHeight();
			auto screenPos = glm::vec2(x, height - y);
			auto world = glm::unProject(glm::vec3(screenPos, 0), view, projection, glm::vec4(0, 0, width, height));
			log("glfw", fmt::format("World pos: {}, {}", world.x, world.y));
		}
	});

	window->addFramebufferSizeCallback([this](int width, int height) {
		updateProjection(width, height);
	});
}

void Camera::update(float dt) {
	// todo cleanup
	auto win = window->getGlfwWindow();
	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
		pos.y -= camSpeed * dt * zoom;
	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
		pos.x += camSpeed * dt * zoom;
	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
		pos.y += camSpeed * dt * zoom;
	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
		pos.x -= camSpeed * dt * zoom;

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
	view = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0));
	view = glm::scale(view, glm::vec3(zoom, zoom, 1.f));
}

void Camera::setPosXY(float x, float y) {
	pos.x = x;
	pos.y = y;
}

void Camera::translate(float x, float y) {
	pos.x -= x;
	pos.y -= y;
}

void Camera::updateProjection(int width, int height) {
	float halfWidth = static_cast<float>(width) / 2.f;
	float halfHeight = static_cast<float>(height) / 2.f;
	projection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1.0f, 1.0f);
}
