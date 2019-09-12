#pragma once

namespace pm {
	class WindowManager;
	class InputManager {
	private:
		WindowManager* window;
	public:
		InputManager(WindowManager* window);
		InputManager();
		~InputManager();
		void update();
	};
}