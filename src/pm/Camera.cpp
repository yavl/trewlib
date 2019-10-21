#include "Camera.hpp"
#include "WindowManager.hpp"
#include <functional>
#include "Logger.hpp"

using namespace pm;

Camera::Camera(std::shared_ptr<WindowManager> window, int width, int height) :
	view(1.0f)
{
	this->window = window;
	camSpeed = 500.0f;
	zoomFactor = 0.15f;
	zoom = 1.0f;
	oldState = GLFW_RELEASE;
	float widthf = static_cast<float>(window->getWidth());
	float heightf = static_cast<float>(window->getHeight());
	projection = glm::ortho(-widthf, widthf, -heightf, heightf, -1.0f, 1.0f);
	projection = glm::scale(projection, glm::vec3(2.0f));

	window->addScrollCallback([this](GLFWwindow* window, double xoffset, double yoffset) {
		if (yoffset > 0)
			zoom += zoomFactor * zoom;
		else
			zoom -= zoomFactor * zoom;
	});

	window->addMouseButtonCallback([=](GLFWwindow* window, int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			auto screenPos = glm::vec2(x, height - y);
			auto world = glm::unProject(glm::vec3(screenPos, 0), view, projection, glm::vec4(0, 0, width, height));
			log("glfw", fmt::format("World pos: {}, {}", world.x, world.y));
		}
	});

	window->addFramebufferSizeCallback([this](GLFWwindow* window, int width, int height) {
		float widthf = static_cast<float>(width);
		float heightf = static_cast<float>(height);
		projection = glm::ortho(-widthf, widthf, -heightf, heightf, -1.0f, 1.0f);
		projection = glm::scale(projection, glm::vec3(2.0f));
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