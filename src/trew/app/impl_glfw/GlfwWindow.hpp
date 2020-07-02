#pragma once

#include "../Window.hpp"
#include <vector>
#include <memory>

struct GLFWwindow;

namespace trew {
	// A wrapper class to handle single window
	class GlfwWindow : public Window {
	public:
		typedef std::function<void(int key, int scancode, int action, int mods)> KeyCallback;

		GlfwWindow();
		virtual ~GlfwWindow();
		void createWindow(const std::string& title, int width, int height) override;
		void swapBuffersPollEvents() override;
		int getWidth() const override;
		int getHeight() const override;
		bool shouldClose() const override;
		void close() override;

		virtual GLFWwindow* getRawGlfwWindow() const;
		void addResizeCallback(std::function<void(int width, int height)>&& resizeCallback) override;
		void addScrollCallback(std::function<void(double xoffset, double yoffset)>&& scrollCallback) override;
		void addMouseButtonCallback(std::function<void(int button, int action, int mods)>&& mouseButtonCallback) override;
		virtual void addKeyCallback(KeyCallback&& f);
		virtual Input& getInput() const override;
	private:
		virtual std::pair<int, int> getSize() const;
		virtual void onResize(int width, int height);
		virtual void onScroll(double xoffset, double yoffset);
		virtual void onMouseButton(int button, int action, int mods);
		virtual void onKey(int key, int scancode, int action, int mods);
		static void framebuffer_size_callback(GLFWwindow*, int width, int height);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

		GLFWwindow* window;
		std::unique_ptr<Input> input;
		std::vector<std::function<void(int width, int height)>> framebufferSizeCallbacks;
		std::vector<std::function<void(double xoffset, double yoffset)>> scrollCallbacks;
		std::vector<std::function<void(int button, int action, int mods)>> mouseButtonCallbacks;
		std::vector<KeyCallback> keyCallbacks;
	};
}
