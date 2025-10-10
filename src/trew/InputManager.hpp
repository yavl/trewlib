#pragma once

namespace trew {
	class Window;
	class Input;
	class InputManager {
	public:
		InputManager(Window* window);
		virtual ~InputManager() = default;
		virtual void update();
	private:
		Window* window;
		Input& input;
	};
}
