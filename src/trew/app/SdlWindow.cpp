#include "SdlWindow.hpp"
#include <trew/Logger.hpp>
#include <trew/input/SdlInput.hpp>
#include <trew/Vector2.hpp>
#include <SDL3/SDL.h>
#include <cassert>

using namespace trew;

#define TREW_USE_OPENGL

SdlWindow::SdlWindow() {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
	}
}

SdlWindow::~SdlWindow() {
	SDL_ReleaseWindowFromGPUDevice(device, window);
	SDL_DestroyWindow(window);
	SDL_DestroyGPUDevice(device);
}

void SdlWindow::createWindow(const std::string& title, int width, int height) {
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

void SdlWindow::swapBuffers() {
	SDL_GL_SwapWindow(getRawSdlWindow());
}

int SdlWindow::getWidth() const {
	return getSize().first;
}

int SdlWindow::getHeight() const {
	return getSize().second;
}

Vector2 SdlWindow::getCursorPos() const {
	SDL_PumpEvents();

	float x, y;
	SDL_GetMouseState(&x, &y);
	return Vector2(x, y);
}

std::pair<int, int> SdlWindow::getSize() const {
	int width;
	int height;
	SDL_GetWindowSize(window, &width, &height);
	return std::make_pair(width, height);
}

bool SdlWindow::shouldClose() const {
	return false;
}

void SdlWindow::close() {
	SDL_DestroyWindow(window);
}

SDL_Window* SdlWindow::getRawSdlWindow() const {
	return window;
}

SDL_GPUDevice* SdlWindow::getSdlGpuDevice() const {
	return device;
}

SDL_GLContext SdlWindow::getSdlContext() const {
	return context;
}

Input& SdlWindow::getInput() const {
	return *input;
}

void SdlWindow::onWindowResize(int width, int height) {
	for (auto f : windowResizeCallbacks) {
		f(width, height);
	}
}

void SdlWindow::onScroll(double xoffset, double yoffset) {
	for (auto f : scrollCallbacks) {
		f(xoffset, yoffset);
	}
}

void SdlWindow::onMouseButton(int button) {
	for (auto f : mouseButtonCallbacks) {
		f(button);
	}
}

void SdlWindow::onKey(int key) {
	for (auto f : keyCallbacks) {
		f(key);
	}
}

void SdlWindow::addWindowResizeCallback(ResizeCallback&& callback) {
	windowResizeCallbacks.emplace_back(callback);
}

void SdlWindow::addScrollCallback(ScrollCallback&& callback) {
	scrollCallbacks.emplace_back(callback);
}

void SdlWindow::addMouseButtonCallback(MouseButtonCallback&& callback) {
	mouseButtonCallbacks.emplace_back(callback);
}

void SdlWindow::addKeyCallback(KeyCallback&& callback) {
	keyCallbacks.emplace_back(callback);
}
