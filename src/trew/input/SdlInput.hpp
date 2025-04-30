#pragma once

#include "Input.hpp"

struct SDL_Window;

namespace trew {
	class SdlInput : public Input {
	public:
		virtual ~SdlInput() = default;
		SdlInput(SDL_Window* window);
		bool isKeyPressed(Key key) const override;
		bool isMousePressed(Key key) const override;
	private:
		SDL_Window* window;
	};
}