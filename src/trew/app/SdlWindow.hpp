#pragma once

#include "Window.hpp"
#include <vector>
#include <memory>
#include <SDL3/SDL_video.h>

struct SDL_Window;
struct SDL_GPUDevice;

namespace trew {
	// A wrapper class to handle single window
	class SdlWindow : public Window {
	public:
		SdlWindow();
		virtual ~SdlWindow();
		void createWindow(const std::string& title, int width, int height) override;
		void swapBuffers() override;
		int getWidth() const override;
		int getHeight() const override;
		Vector2 getCursorPos() const override;
		bool shouldClose() const override;
		void close() override;

		virtual SDL_Window* getRawSdlWindow() const;
		virtual SDL_GPUDevice* getSdlGpuDevice() const;
		virtual SDL_GLContext getSdlContext() const;
		virtual Input& getInput() const override;
		virtual void onWindowResize(int width, int height);
		virtual void onScroll(double xoffset, double yoffset);
		virtual void onMouseButton(int button);
		virtual void onKey(int key);

		void addWindowResizeCallback(ResizeCallback&& callback) override;
		void addScrollCallback(ScrollCallback&& callback) override;
		void addMouseButtonCallback(MouseButtonCallback&& callback) override;
		void addKeyCallback(KeyCallback&& callback) override;
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
