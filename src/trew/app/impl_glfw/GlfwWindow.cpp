#include "GlfwWindow.hpp"
#include <trew/trew.hpp>
#include <trew/Logger.hpp>
#include <trew/input/impl_glfw/GlfwInput.hpp>
#include <trew/Vector2.hpp>

using namespace trew;

GlfwWindow::GlfwWindow() {
	window = nullptr;
}

GlfwWindow::~GlfwWindow() {
	glfwTerminate();
}

void GlfwWindow::createWindow(const std::string& title, int width, int height) {
	assert(window == nullptr);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!window) {
		logError("GlfwWindow", "Failed to create GLFW window");
		glfwTerminate();
	}

	glfwSetWindowUserPointer(window, this);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(window, (mode->width - width) / 2, (mode->height - height) / 2);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetKeyCallback(window, key_callback);
	input = std::make_unique<GlfwInput>(window);
}

void GlfwWindow::swapBuffersPollEvents() {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

int GlfwWindow::getWidth() const {
	return getSize().first;
}

int GlfwWindow::getHeight() const {
	return getSize().second;
}

Vector2 GlfwWindow::getCursorPos() const {
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	return Vector2(x, y);
}

std::pair<int, int> GlfwWindow::getSize() const {
	int width;
	int height;
	glfwGetWindowSize(window, &width, &height);
	return std::make_pair(width, height);
}

bool GlfwWindow::shouldClose() const {
	return glfwWindowShouldClose(window);
}

void GlfwWindow::close() {
	glfwSetWindowShouldClose(window, true);
}

GLFWwindow* GlfwWindow::getRawGlfwWindow() const {
	return window;
}

void GlfwWindow::addResizeCallback(std::function<void(int width, int height)>&& resizeCallback) {
	framebufferSizeCallbacks.emplace_back(resizeCallback);
}

void GlfwWindow::addScrollCallback(std::function<void(double xoffset, double yoffset)>&& scrollCallback) {
	scrollCallbacks.emplace_back(scrollCallback);
}

void GlfwWindow::addMouseButtonCallback(std::function<void(int button, int action, int mods)>&& mouseButtonCallback) {
	mouseButtonCallbacks.emplace_back(mouseButtonCallback);
}

void GlfwWindow::addKeyCallback(KeyCallback&& f) {
	keyCallbacks.emplace_back(f);
}

Input& GlfwWindow::getInput() const {
	return *input;
}

void GlfwWindow::onResize(int width, int height) {
	for (auto f : framebufferSizeCallbacks) {
		f(width, height);
	}
}

void GlfwWindow::onScroll(double xoffset, double yoffset) {
	for (auto f : scrollCallbacks) {
		f(xoffset, yoffset);
	}
}

void GlfwWindow::onMouseButton(int button, int action, int mods) {
	for (auto f : mouseButtonCallbacks) {
		f(button, action, mods);
	}
}

void GlfwWindow::onKey(int key, int scancode, int action, int mods) {
	for (auto f : keyCallbacks) {
		f(key, scancode, action, mods);
	}
}

void GlfwWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window))->onResize(width, height);
}

void GlfwWindow::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window))->onScroll(xoffset, yoffset);
}

void GlfwWindow::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window))->onMouseButton(button, action, mods);
}

void GlfwWindow::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window))->onKey(key, scancode, action, mods);
}
