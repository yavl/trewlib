#include "InputManager.hpp"
#include <trew/input/Input.hpp>
#include <trew/app/Window.hpp>
#include <SDL3/SDL.h>
#include <fmt/core.h>

using namespace trew;

InputManager::InputManager(Window* window) :
	input(window->getInput()),
	window(window)
{
}

void InputManager::update() {
	if (input.isKeyPressed(Key::ESCAPE)) {
		auto event = SDL_Event();
		event.type = SDL_EVENT_QUIT;
		SDL_PushEvent(&event);
	}
}