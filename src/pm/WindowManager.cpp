#include "WindowManager.hpp"
#include "pm.hpp"

using namespace pm;

WindowManager::WindowManager() {
	window = nullptr;
}

WindowManager::~WindowManager() {
	glfwTerminate();
}

void WindowManager::createWindow(std::string title, int width, int height) {
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
		fmt::print("Failed to create GLFW window");
		glfwTerminate();
	}

	glfwSetWindowUserPointer(window, this);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(window, (mode->width - width) / 2, (mode->height - height) / 2);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void WindowManager::onResize(int width, int height) {
	glViewport(0, 0, width, height);
	fmt::print("on resize\n");
}

int WindowManager::getWidth() {
	return getSize().first;
}

int WindowManager::getHeight() {
	return getSize().second;
}

std::pair<int, int> pm::WindowManager::getSize() {
	int width;
	int height;
	glfwGetFramebufferSize(window, &width, &height);
	return std::make_pair(width, height);
}

bool WindowManager::shouldClose() {
	return glfwWindowShouldClose(window);
}

GLFWwindow* WindowManager::getGlfwWindow() {
	return window;
}

void WindowManager::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	static_cast<WindowManager*>(glfwGetWindowUserPointer(window))->onResize(width, height);
}

