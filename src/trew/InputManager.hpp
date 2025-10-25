#pragma once

#include <SDL3/SDL_events.h>

namespace trew {
	class Window;
	class Input;
	class InputManager {
	public:
		InputManager(Window* window);
		virtual ~InputManager() = default;
		virtual void update(SDL_Event event);
	private:
		Window* window;
		Input& input;
	};
}
