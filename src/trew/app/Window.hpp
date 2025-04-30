#pragma once

#include <string>
#include <functional>

namespace trew {
	class Input;
	class Vector2;
	class Window {
	public:
		typedef std::function<void(int width, int height)> ResizeCallback;
		typedef std::function<void(double xoffset, double yoffset)> ScrollCallback;
		typedef std::function<void(int button)> MouseButtonCallback;
		typedef std::function<void(int key)> KeyCallback;

		virtual ~Window() = default;
		virtual void createWindow(const std::string& title, int width, int height) = 0;
		virtual void swapBuffers() = 0;
		virtual int getWidth() const = 0;
		virtual int getHeight() const = 0;
		virtual Vector2 getCursorPos() const = 0;
		virtual bool shouldClose() const = 0;
		virtual void close() = 0;
		virtual Input& getInput() const = 0;

		virtual void addWindowResizeCallback(ResizeCallback&& resizeCallback) = 0;
		virtual void addScrollCallback(ScrollCallback&& scrollCallback) = 0;
		virtual void addMouseButtonCallback(MouseButtonCallback&& mouseButtonCallback) = 0;
		virtual void addKeyCallback(KeyCallback&& keyCallback) = 0;
	};
}