#include "Game.hpp"
#include <trew/Globals.hpp>
#include <trew/InputManager.hpp>
#include <trew/Camera.hpp>
#include <trew/scripting/ASManager.hpp>
#include <trew/AssetManager.hpp>
#include <trew/app/Window.hpp>
#include <SDL3/SDL.h>
#include <trew/Shader.hpp>
#include <trew/Hud.hpp>
#include <trew/graphics/Renderer.hpp>
#include <trew/nodes/Sprite.hpp>
#include <trew/drawables/ImageSurface.hpp>
#include <iostream>
#include <fmt/core.h>

Game::Game(Window* window) :
	window(window)
{
	cam = std::make_unique<Camera>(window);
	input = std::make_unique<InputManager>(window);
	Globals::camera = cam.get();
}

Game::~Game() {

}

void Game::create() {
	assets = std::make_unique<AssetManager>("assets");
	assets->load("tex.png", AssetType::IMAGE);
	assets->load("tex2.png", AssetType::IMAGE);
	assets->load("circle.png", AssetType::IMAGE);

	ASManager as(assets.get(), cam.get());
	//as.registerScript("assets/scripts/main.as");
	//as.runScript("assets/scripts/main.as");

	auto device = window->getSdlGpuDevice();
    
    hud = std::make_unique<Hud>(window);
	renderer = std::make_unique<Renderer>(device, window->getRawSdlWindow(), cam.get(), assets.get());
	renderer->init();

	// Register system
	auto sys = world.system<Position, Velocity>()
		.each([](flecs::iter& it, size_t, Position& p, Velocity& v) {
			p.x += v.x * it.delta_time();
			p.y += v.y * it.delta_time();
		});

	// Create an entity with name Bob, add Position and food preference
	world.entity("Bob")
		.set(Position{ 0, 0 })
		.set(Velocity{ 50.f, 50.f })
		.set(Image{ assets->getImage("circle.png")})
		.add<Eats, Apples>();

	query = world.query<Position, Image>();
}

void Game::update(float dt) {
	cam->update(dt);
	world.progress(dt);

	SDL_Event event;
	//fmt::print("fps: {}\n", 1 / dt);
	while (SDL_PollEvent(&event)) {
		input->update(event);
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
	renderer->render(hud.get());
	query.each([this](flecs::entity e, Position& pos, Image& p) {
		renderer->drawTexture(pos.x, pos.y, p.image->getImageWidth(), p.image->getImageHeight(), renderer->getTexture(p.image), 0.f);
		});
	renderer->submit();
}

void Game::resize(int width, int height) {
	render();
}
