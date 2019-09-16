#pragma once

namespace pm {
	class WindowManager;
	class InputManager {
	private:
		WindowManager* window;
	public:
		InputManager(WindowManager* window);
		InputManager() = default;
		virtual ~InputManager() = default;
		virtual void update();
	};
}