#pragma once

#include <string>

struct GLFWwindow;

namespace pm {
	// A wrapper class to handle single window
	class WindowManager {
	private:
		GLFWwindow* window;
	public:
		WindowManager();
		virtual ~WindowManager();
		virtual void createWindow(std::string title, int width, int height);
		virtual void onResize(int width, int height);
		virtual int getWidth();
		virtual int getHeight();
		virtual std::pair<int, int> getSize();
		virtual bool shouldClose();
		virtual GLFWwindow* getGlfwWindow();
		static void framebuffer_size_callback(GLFWwindow*, int width, int height);
	};
}