#pragma once

#include <memory>

namespace trew {
	class Window;
	class Input;
	class InputManager {
	public:
		InputManager(std::shared_ptr<Window> window);
		virtual ~InputManager() = default;
		virtual void update();
	private:
		std::shared_ptr<Window> window;
		Input& input;
	};
}
