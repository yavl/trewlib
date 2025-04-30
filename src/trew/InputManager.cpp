#include "InputManager.hpp"
#include <trew/app/Window.hpp>
#include <trew/input/Input.hpp>
#include <SDL3/SDL.h>
#include <fmt/core.h>

using namespace trew;

InputManager::InputManager(std::weak_ptr<Window> window) : input(window.lock()->getInput()) {
	this->window = window;
}

void InputManager::update() {
	if (input.isKeyPressed(Key::ESCAPE)) {
		auto event = SDL_Event();
		event.type = SDL_EVENT_QUIT;
		SDL_PushEvent(&event);
	}
}