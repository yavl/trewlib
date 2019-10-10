#pragma once

namespace pm {
	class WindowManager;
	class InputManager {
	public:
		InputManager(WindowManager* window);
		virtual ~InputManager() = default;
		virtual void update();
	private:
		WindowManager* window;
	};
}
