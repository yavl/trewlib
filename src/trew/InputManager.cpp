#include "InputManager.hpp"
#include <trew/input/Input.hpp>
#include <trew/app/Window.hpp>
#include <SDL3/SDL.h>
#include <fmt/core.h>
#include <trew/Globals.hpp>
#include <trew/Camera.hpp>

using namespace trew;

InputManager::InputManager(Window* window) :
	input(window->getInput()),
	window(window)
{
}

void InputManager::update(SDL_Event event) {
	if (input.isKeyPressed(Key::ESCAPE)) {
		auto event = SDL_Event();
		event.type = SDL_EVENT_QUIT;
		SDL_PushEvent(&event);
	}
	if (event.button.button == SDL_BUTTON_LEFT) {
		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
			fmt::print("Mouse button pressed! {}, {}\n", event.button.x, event.button.y);

			auto vec = Globals::camera->screenToSpace(event.button.x, event.button.y);
			Globals::selectionQuad[0] = vec;
			Globals::hasSelectionQuad = false;
		} else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
			fmt::print("Mouse button released! {}, {}\n", event.button.x, event.button.y);

			auto vec = Globals::camera->screenToSpace(event.button.x, event.button.y);
			Globals::selectionQuad[1] = vec;
			Globals::hasSelectionQuad = true;
		}
	}
}