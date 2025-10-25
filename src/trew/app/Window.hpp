#pragma once

#include <vector>
#include <memory>
#include <SDL3/SDL_video.h>
#include <string>
#include <functional>
#include <trew/Vector2.hpp>

struct SDL_Window;
struct SDL_GPUDevice;

namespace trew {
	class Input;
	// A wrapper class to handle single window
	class Window {
	public:
		typedef std::function<void(int width, int height)> ResizeCallback;
		typedef std::function<void(double xoffset, double yoffset)> ScrollCallback;
		typedef std::function<void(int button)> MouseButtonCallback;
		typedef std::function<void(int key)> KeyCallback;

		Window();
		virtual ~Window();
		void createWindow(const std::string& title, int width, int height);
		int getWidth() const;
		int getHeight() const;
		Vector2 getCursorPos() const;
		bool shouldClose() const;
		void close();

		virtual SDL_Window* getRawSdlWindow() const;
		virtual SDL_GPUDevice* getSdlGpuDevice() const;
		virtual SDL_GLContext getSdlContext() const;
		virtual Input& getInput() const;
		virtual void onWindowResize(int width, int height);
		virtual void onScroll(double xoffset, double yoffset);
		virtual void onMouseButton(int button);
		virtual void onKey(int key);

		void addWindowResizeCallback(ResizeCallback&& callback);
		void addScrollCallback(ScrollCallback&& callback);
		void addMouseButtonCallback(MouseButtonCallback&& callback);
		void addKeyCallback(KeyCallback&& callback);
	private:
		virtual std::pair<int, int> getSize() const;

		SDL_Window* window = nullptr;
		SDL_GPUDevice* device = nullptr;
		SDL_GLContext context = nullptr;
		std::unique_ptr<Input> input;
		std::vector<ResizeCallback> windowResizeCallbacks;
		std::vector<ScrollCallback> scrollCallbacks;
		std::vector<MouseButtonCallback> mouseButtonCallbacks;
		std::vector<KeyCallback> keyCallbacks;
	};
}
