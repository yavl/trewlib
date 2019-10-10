#pragma once

#include <string>

struct GLFWwindow;

namespace pm {
	// A wrapper class to handle single window
	class WindowManager {
	public:
		WindowManager();
		virtual ~WindowManager();
		virtual void createWindow(std::string title, int width, int height);
		virtual void onResize(int width, int height);
		virtual int getWidth() const;
		virtual int getHeight() const;
		virtual std::pair<int, int> getSize() const;
		virtual bool shouldClose() const;
		virtual GLFWwindow* getGlfwWindow() const;
		static void framebuffer_size_callback(GLFWwindow*, int width, int height);
	private:
		GLFWwindow* window;
	};
}