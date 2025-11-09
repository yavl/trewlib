#include "Camera.hpp"
#include <trew/Logger.hpp>
#include <trew/input/Input.hpp>
#include <trew/app/Window.hpp>
#include <functional>

using namespace trew;

constexpr auto LOGTAG = "Camera";

const float CAMERA_SPEED = 500.f;

Camera::Camera(Window* window) :
	view(1.f),
	window(window)
{
	camSpeed = CAMERA_SPEED;
	zoomFactor = 0.15f;
	zoom = 1.f;
	oldMousePressed = false;
	updateProjection(window->getWidth(), window->getHeight());

	window->addScrollCallback([this, window](double xoffset, double yoffset) {
		if (yoffset > 0) {
			zoom += zoomFactor * zoom;
		}
		else {
			zoom -= zoomFactor * zoom;
		}
	});

	window->addWindowResizeCallback([this](int width, int height) {
		updateProjection(width, height);
	});
}

void Camera::update(float dt) {
	auto& input = window->getInput();

	if (input.isKeyPressed(Key::W))
		pos.y += camSpeed * dt;
	if (input.isKeyPressed(Key::A))
		pos.x -= camSpeed * dt;
	if (input.isKeyPressed(Key::S))
		pos.y -= camSpeed * dt;
	if (input.isKeyPressed(Key::D))
		pos.x += camSpeed * dt;

	if (input.isMousePressed(Key::LEFT_MOUSE_BUTTON)) {
		auto pos = window->getCursorPos();
		auto world = screenToSpace(pos.x, pos.y);
		//log(LOGTAG, fmt::format("World pos: {}, {}", world.x, world.y));
	}

	bool mousePressed = false;
	if (input.isMousePressed(Key::MIDDLE_MOUSE_BUTTON)) {
		mousePressed = true;
	}

	auto cursorPos = window->getCursorPos();

	float mouseX = cursorPos.x;
	float mouseY = cursorPos.y;
	if (mousePressed && !oldMousePressed) {
		dragNew = Vector2(mouseX, mouseY);
		dragNew *= 1.f / zoom;
		dragOld = dragNew;
	}
	if (mousePressed) {
		dragNew = Vector2(mouseX, mouseY);
		dragNew *= 1.f / zoom;
		if (dragNew != dragOld) {
			translate(dragOld.x - dragNew.x, dragNew.y - dragOld.y);
			dragOld = dragNew;
		}		
	}
	oldMousePressed = mousePressed;

	glm::vec3 cameraPos = glm::vec3(pos.x, pos.y, 1.f);
	glm::vec3 cameraFront = glm::vec3(0.f, 0.f, 1.f);
	glm::vec3 cameraUp = glm::vec3(0.f, -1.f, 0.f);

	view = glm::lookAt(
		cameraPos,
		cameraPos + cameraFront,
		cameraUp);

	auto asd = screenToSpace(window->getWidth() / 2.f, window->getHeight() / 2.f);
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
