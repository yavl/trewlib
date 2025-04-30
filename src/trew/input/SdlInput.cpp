#include "SdlInput.hpp"
#include <SDL3/SDL_events.h>

using namespace trew;

SdlInput::SdlInput(SDL_Window* window) {
	this->window = window;
}

bool SdlInput::isKeyPressed(Key key) const {
	SDL_PumpEvents();

	auto keys = SDL_GetKeyboardState(nullptr);
	switch (key) {
	case Key::ESCAPE: {
		return keys[SDL_SCANCODE_ESCAPE];
		break;
	}
	case Key::W: {
		return keys[SDL_SCANCODE_W];
		break;
	}
	case Key::A: {
		return keys[SDL_SCANCODE_A];
		break;
	}
	case Key::S: {
		return keys[SDL_SCANCODE_S];
		break;
	}
	case Key::D: {
		return keys[SDL_SCANCODE_D];
		break;
	}
	default:
		return false;
	}
}

bool SdlInput::isMousePressed(Key key) const {
	SDL_PumpEvents();

	float x, y;
	auto button = SDL_GetMouseState(&x, &y);
	switch (key) {
	case Key::LEFT_MOUSE_BUTTON: {
		return button == SDL_BUTTON_LEFT;
		break;
	}
	case Key::MIDDLE_MOUSE_BUTTON: {
		return button == SDL_BUTTON_MIDDLE;
		break;
	}
	default:
		return false;
	}
}