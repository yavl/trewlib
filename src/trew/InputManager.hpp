#pragma once

#include <memory>

namespace trew {
	class WindowManager;
	class InputManager {
	public:
		InputManager(std::shared_ptr<WindowManager> window);
		virtual ~InputManager() = default;
		virtual void update();
	private:
		std::shared_ptr<WindowManager> window;
	};
}
