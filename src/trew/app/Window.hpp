#pragma once

#include <string>
#include <functional>

namespace trew {
	class Input;
	class Window {
	public:
		virtual ~Window() = default;
		virtual void createWindow(const std::string& title, int width, int height) = 0;
		virtual void swapBuffersPollEvents() = 0;
		virtual int getWidth() const = 0;
		virtual int getHeight() const = 0;
		virtual bool shouldClose() const = 0;
		virtual void close() = 0;
		virtual void addResizeCallback(std::function<void(int width, int height)>&& resizeCallback) = 0;
		virtual void addScrollCallback(std::function<void(double xoffset, double yoffset)>&& scrollCallback) = 0;
		virtual void addMouseButtonCallback(std::function<void(int button, int action, int mods)>&& mouseButtonCallback) = 0;
		virtual Input& getInput() const = 0;
	};
}