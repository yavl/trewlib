#include "Camera.hpp"
#include <trew/app/impl_glfw/GlfwWindow.hpp>
#include <trew/Logger.hpp>
#include <trew/input/Input.hpp>
#include <functional>

using namespace trew;

const int CAMERA_SPEED = 500.f;

Camera::Camera(std::weak_ptr<Window> window) :
	view(1.f)
{
	this->window = window;
	camSpeed = CAMERA_SPEED;
	zoomFactor = 0.15f;
	zoom = 1.f;
	oldState = GLFW_RELEASE;
	updateProjection(window.lock()->getWidth(), window.lock()->getHeight());

	window.lock()->addScrollCallback([this](double xoffset, double yoffset) {
		if (yoffset > 0)
			zoom += zoomFactor * zoom;
		else
			zoom -= zoomFactor * zoom;
	});

	window.lock()->addMouseButtonCallback([=](int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			double x, y;
			auto win = static_cast<GlfwWindow*>(window.lock().get());
			glfwGetCursorPos(win->getRawGlfwWindow(), &x, &y);
			auto world = screenToSpace(static_cast<float>(x), static_cast<float>(y));
			log("glfw", fmt::format("World pos: {}, {}", world.x, world.y));
		}
	});

	window.lock()->addResizeCallback([this](int width, int height) {
		updateProjection(width, height);
	});
}

void Camera::update(float dt) {
	// todo cleanup & make it implementation aware (no glfw)
	auto win = static_cast<GlfwWindow*>(window.lock().get())->getRawGlfwWindow();
	auto& input = window.lock().get()->getInput();

	if (input.isKeyPressed(Key::W))
		pos.y += camSpeed * dt;
	if (input.isKeyPressed(Key::A))
		pos.x -= camSpeed * dt;
	if (input.isKeyPressed(Key::S))
		pos.y -= camSpeed * dt;
	if (input.isKeyPressed(Key::D))
		pos.x += camSpeed * dt;

	int state = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_MIDDLE);
	double xpos, ypos;
	glfwGetCursorPos(win, &xpos, &ypos);
	float mouseX = static_cast<float>(xpos);
	float mouseY = static_cast<float>(ypos);
	if (state == GLFW_PRESS && oldState == GLFW_RELEASE) {
		dragNew = Vector2(mouseX, mouseY);
		dragNew *= 1.f / zoom;
		dragOld = dragNew;
	}
	if (state == GLFW_PRESS) {
		dragNew = Vector2(mouseX, mouseY);
		dragNew *= 1.f / zoom;
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

	auto asd = screenToSpace(window.lock()->getWidth() / 2.f, window.lock()->getHeight() / 2.f);
	glm::vec3 center(asd.x, asd.y, 0.f);
	view = glm::translate(view, glm::vec3(center.x, center.y, 0.f));
	view = glm::scale(view, glm::vec3(zoom, zoom, 1.f));
	view = glm::translate(view, glm::vec3(-center.x, -center.y, 0.f));
	camSpeed = CAMERA_SPEED * 1.f/zoom;
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
	int width = window.lock()->getWidth();
	int height = window.lock()->getHeight();
	auto screenPos = glm::vec2(x, height - y);
	auto world = glm::unProject(glm::vec3(screenPos, 0), view, projection, glm::vec4(0, 0, width, height));
	return Vector2(world.x, world.y);
}

void Camera::updateProjection(int width, int height) {
	float halfWidth = static_cast<float>(width) / 2.f;
	float halfHeight = static_cast<float>(height) / 2.f;
	projection = glm::ortho(-halfWidth, halfWidth, halfHeight, -halfHeight, -1.f, 1.f);
}
