#include "Window.hpp"
#include <trew/Logger.hpp>
#include <trew/input/SdlInput.hpp>
#include <trew/Vector2.hpp>
#include <SDL3/SDL.h>
#include <cassert>

using namespace trew;

//#define TREW_USE_OPENGL

Window::Window() {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
	}
}

Window::~Window() {
	SDL_ReleaseWindowFromGPUDevice(device, window);
	SDL_DestroyWindow(window);
	SDL_DestroyGPUDevice(device);
}

void Window::createWindow(const std::string& title, int width, int height) {
	assert(window == nullptr);

	device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL, true, nullptr);
	if (!device) {
		SDL_Log("SDL_CreateGPUDevice failed: %s", SDL_GetError());
		SDL_Quit();
	}
#ifdef TREW_USE_OPENGL
	window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	if (!window) {
		SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
		SDL_Quit();
	}
	context = SDL_GL_CreateContext(window);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#else
	window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_RESIZABLE);
	if (!window) {
		SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
		SDL_Quit();
	}
#endif

	if (!SDL_ClaimWindowForGPUDevice(device, window)) {
		SDL_Log("SDL_ClaimWindowForGPUDevice failed %s", SDL_GetError());
		SDL_Quit();
	}

	input = std::make_unique<SdlInput>(window);
}

void Window::swapBuffers() {
#ifdef TREW_USE_OPENGL
	SDL_GL_SwapWindow(getRawSdlWindow());
#endif
}

int Window::getWidth() const {
	return getSize().first;
}

int Window::getHeight() const {
	return getSize().second;
}

Vector2 Window::getCursorPos() const {
	SDL_PumpEvents();

	float x, y;
	SDL_GetMouseState(&x, &y);
	return Vector2(x, y);
}

std::pair<int, int> Window::getSize() const {
	int width;
	int height;
	SDL_GetWindowSize(window, &width, &height);
	return std::make_pair(width, height);
}

bool Window::shouldClose() const {
	return false;
}

void Window::close() {
	SDL_DestroyWindow(window);
}

SDL_Window* Window::getRawSdlWindow() const {
	return window;
}

SDL_GPUDevice* Window::getSdlGpuDevice() const {
	return device;
}

SDL_GLContext Window::getSdlContext() const {
	return context;
}

Input& Window::getInput() const {
	return *input;
}

void Window::onWindowResize(int width, int height) {
	for (auto f : windowResizeCallbacks) {
		f(width, height);
	}
}

void Window::onScroll(double xoffset, double yoffset) {
	for (auto f : scrollCallbacks) {
		f(xoffset, yoffset);
	}
}

void Window::onMouseButton(int button) {
	for (auto f : mouseButtonCallbacks) {
		f(button);
	}
}

void Window::onKey(int key) {
	for (auto f : keyCallbacks) {
		f(key);
	}
}

void Window::addWindowResizeCallback(ResizeCallback&& callback) {
	windowResizeCallbacks.emplace_back(callback);
}

void Window::addScrollCallback(ScrollCallback&& callback) {
	scrollCallbacks.emplace_back(callback);
}

void Window::addMouseButtonCallback(MouseButtonCallback&& callback) {
	mouseButtonCallbacks.emplace_back(callback);
}

void Window::addKeyCallback(KeyCallback&& callback) {
	keyCallbacks.emplace_back(callback);
}
