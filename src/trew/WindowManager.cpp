#include "WindowManager.hpp"
#include <trew/trew.hpp>
#include <trew/Logger.hpp>
#include <trew/input/impl/GlfwInput.hpp>

using namespace trew;

WindowManager::WindowManager() {
	window = nullptr;
}

WindowManager::~WindowManager() {
	glfwTerminate();
}

void WindowManager::createWindow(const std::string& title, int width, int height) {
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
		logError("WindowManager", "Failed to create GLFW window");
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

void WindowManager::swapBuffersPollEvents() {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

int WindowManager::getWidth() const {
	return getSize().first;
}

int WindowManager::getHeight() const {
	return getSize().second;
}

std::pair<int, int> WindowManager::getSize() const {
	int width;
	int height;
	glfwGetWindowSize(window, &width, &height);
	return std::make_pair(width, height);
}

bool WindowManager::shouldClose() const {
	return glfwWindowShouldClose(window);
}

void WindowManager::close() {
	glfwSetWindowShouldClose(window, true);
}

GLFWwindow* WindowManager::getGlfwWindow() const {
	return window;
}

void WindowManager::addResizeCallback(std::function<void(int width, int height)>&& resizeCallback) {
	framebufferSizeCallbacks.emplace_back(resizeCallback);
}

void WindowManager::addScrollCallback(std::function<void(double xoffset, double yoffset)>&& scrollCallback) {
	scrollCallbacks.emplace_back(scrollCallback);
}

void WindowManager::addMouseButtonCallback(std::function<void(int button, int action, int mods)>&& mouseButtonCallback) {
	mouseButtonCallbacks.emplace_back(mouseButtonCallback);
}

void WindowManager::addKeyCallback(KeyCallback&& f) {
	keyCallbacks.emplace_back(f);
}

Input& WindowManager::getInput() const {
	return *input;
}

void WindowManager::onResize(int width, int height) {
	for (auto f : framebufferSizeCallbacks) {
		f(width, height);
	}
}

void WindowManager::onScroll(double xoffset, double yoffset) {
	for (auto f : scrollCallbacks) {
		f(xoffset, yoffset);
	}
}

void WindowManager::onMouseButton(int button, int action, int mods) {
	for (auto f : mouseButtonCallbacks) {
		f(button, action, mods);
	}
}

void WindowManager::onKey(int key, int scancode, int action, int mods) {
	for (auto f : keyCallbacks) {
		f(key, scancode, action, mods);
	}
}

void WindowManager::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	static_cast<WindowManager*>(glfwGetWindowUserPointer(window))->onResize(width, height);
}

void WindowManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	static_cast<WindowManager*>(glfwGetWindowUserPointer(window))->onScroll(xoffset, yoffset);
}

void WindowManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	static_cast<WindowManager*>(glfwGetWindowUserPointer(window))->onMouseButton(button, action, mods);
}

void WindowManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	static_cast<WindowManager*>(glfwGetWindowUserPointer(window))->onKey(key, scancode, action, mods);
}
