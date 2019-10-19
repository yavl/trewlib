#pragma once

#include <string>
#include <vector>
#include <functional>

struct GLFWwindow;

namespace pm {
	// A wrapper class to handle single window
	class WindowManager {
	public:
		typedef std::function<void(GLFWwindow* window, int width, int height)> FramebufferSizeCallback;
		typedef std::function<void(GLFWwindow* window, double xoffset, double yoffset)> ScrollCallback;
		typedef std::function<void(GLFWwindow* window, int button, int action, int mods)> MouseButtonCallback;
		typedef std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)> KeyCallback;

		WindowManager();
		virtual ~WindowManager();
		virtual void createWindow(std::string title, int width, int height);
		virtual int getWidth() const;
		virtual int getHeight() const;
		virtual std::pair<int, int> getSize() const;
		virtual bool shouldClose() const;
		virtual GLFWwindow* getGlfwWindow() const;
		virtual void addFramebufferSizeCallback(FramebufferSizeCallback&& f);
		virtual void addScrollCallback(ScrollCallback&& f);
		virtual void addMouseButtonCallback(MouseButtonCallback&& f);
		virtual void addKeyCallback(KeyCallback&& f);
	private:
		virtual void onResize(int width, int height);
		virtual void onScroll(double xoffset, double yoffset);
		virtual void onMouseButton(int button, int action, int mods);
		virtual void onKey(int key, int scancode, int action, int mods);
		static void framebuffer_size_callback(GLFWwindow*, int width, int height);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

		GLFWwindow* window;
		std::vector<FramebufferSizeCallback> framebufferSizeCallbacks;
		std::vector<ScrollCallback> scrollCallbacks;
		std::vector<MouseButtonCallback> mouseButtonCallbacks;
		std::vector<KeyCallback> keyCallbacks;
	};
}