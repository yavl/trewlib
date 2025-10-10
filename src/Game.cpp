#include "Game.hpp"
#include <trew/Globals.hpp>
#include <trew/InputManager.hpp>
#include <trew/Camera.hpp>
#include <trew/scripting/ASManager.hpp>
#include <trew/AssetManager.hpp>
#include <trew/app/Window.hpp>
#include <SDL3/SDL.h>
#include <fmt/core.h>
#include <trew/Shader.hpp>
#include <trew/Hud.hpp>
#include <trew/graphics/Renderer.hpp>
#include <trew/nodes/Sprite.hpp>
#include <trew/drawables/ImageSurface.hpp>

static Sint32 ResolutionIndex;

Game::Game(Window* window) :
	window(window)
{
	cam = std::make_unique<Camera>(window);
	input = std::make_unique<InputManager>(window);
}

Game::~Game() {

}

void Game::create() {
	assets = std::make_unique<AssetManager>("assets");
	assets->load("tex.png", AssetType::IMAGE);

	auto surface = assets->getImage("tex.png");
	sprite = std::make_unique<Sprite>(surface);
	sprite->setXY(-1010, 0);
	
	auto sprite2 = new Sprite(surface);
	sprite2->setXY(400, 300);
	sprite->addChild(sprite2);

	ASManager as(assets.get(), cam.get());
	//as.registerScript("assets/scripts/main.as");
	//as.runScript("assets/scripts/main.as");

	ResolutionIndex = 0;

	auto sdlWindow = static_cast<Window*>(window);
	auto device = sdlWindow->getSdlGpuDevice();
    
    hud = std::make_unique<Hud>(sdlWindow);
	renderer = std::make_unique<Renderer>(device, sdlWindow->getRawSdlWindow(), cam.get(), assets.get());
	renderer->init();
}

void Game::update(float dt) {
	input->update();
	cam->update(dt);

	SDL_Event event;
	//fmt::println("fps: {}", 1 / dt);
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			SDL_Quit();
			exit(0);
		}
		if (event.type == SDL_EVENT_MOUSE_WHEEL) {
			window->onScroll(event.wheel.x, event.wheel.y);
		}
		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
			window->onMouseButton(event.button.button);
		}
		if (event.type == SDL_EVENT_KEY_DOWN) {
			window->onKey(event.button.button);
		}
		if (event.type == SDL_EVENT_WINDOW_RESIZED) {
			int width, height;
			SDL_GetWindowSize(window->getRawSdlWindow(), &width, &height);
			window->onWindowResize(width, height);
		}
        hud.get()->update(event);
	}
}

void Game::render() {
	renderer->render();
	sprite->draw(renderer.get());
	renderer->render(hud.get());
	renderer->submit();
}

void Game::resize(int width, int height) {
	render();
}
