# Trewlib
A very tiny cross-platform 2D game library I plan to use for my dream 2D game for Windows & macOS. This is not supposed to be a general-purpose game library. Very WIP, not recommended for use at this time.
### Features
* SDL3 GPU renderer
* Compiles on Windows, macOS (Linux not tested but also should)
* AngelScript as a scripting language (WIP)
* [Slang shaders](https://github.com/shader-slang/slang)
* Does nothing except drawing sprites, text, loading shaders and running scripts
### Dependencies
SDL3, glm, flecs, fmt, enet, AngelScript
### Building on Windows
Go to `build` folder and run `./generateWin64.bat` (CMake and Visual Studio 2022 must be installed).
### Building on macOS
```
brew install slangc
brew install sdl3
brew install sdl3_ttf
brew install freetype
```
Go to `build` folder and run `cmake . -G Xcode` (XCode and command-line tools must be installed). Open and build `Trewlib.xcodeproj`, copy `build/assets` folder next to an executable and run.
### Example
```c++
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
#include <trew/graphics/GraphicsTypes.hpp>
#include <trew/Logger.hpp>

constexpr auto LOGTAG = "Game";

const char* mapName = "default-europe";

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
	auto terrainImagePath = assets->loadMap("default-europe");

	ASManager as(assets.get(), cam.get());
	//as.registerScript("assets/scripts/main.as");
	//as.runScript("assets/scripts/main.as");

	auto device = window->getSdlGpuDevice();
    
    hud = std::make_unique<Hud>(window);
	renderer = std::make_unique<Renderer>(device, window->getRawSdlWindow(), cam.get(), assets.get());
	renderer->init();

	renderer->prepareTexture("tex.png");
	renderer->prepareTexture("tex2.png");
	renderer->prepareTexture("circle.png");
	renderer->prepareTexture(terrainImagePath.c_str());

	world.entity("map")
		.set(Image{ assets->getTerrain("default-europe")})
		.set(MapIdentity{ "default-europe"});

	world.entity("character1")
		.set(Position{ 0, 0 })
		.set(Velocity{ 50.f, 50.f })
		.set(Image{ assets->getImage("circle.png")})
		.set(CharacterIdentity{ "Daniel" })

	mapQuery = world.query<Image, MapIdentity>();
	characterQuery = world.query<Position, Image, CharacterIdentity>();

	auto movingSystem = world.system<Position, Velocity>()
		.each([](flecs::iter& it, size_t, Position& p, Velocity& v) {
		p.x += v.x * it.delta_time();
		p.y += v.y * it.delta_time();
	});
}

void Game::update(float dt) {
	cam->update(dt);
	world.progress(dt);

	SDL_Event event;
	//logDebug(LOGTAG, fmt::format("fps: {}", 1 / dt));
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
	mapQuery.each([this](flecs::entity e, Image& p, MapIdentity map) {
		renderer->drawTexture(0.f, 0.f, p.image->getImageWidth(), p.image->getImageHeight(), renderer->getTexture(p.image), 0.f, 128.f);
		});
	characterQuery.each([this](flecs::entity e, Position& pos, Image& p, CharacterIdentity& identity) {
		renderer->drawTexture(pos.x, pos.y, p.image->getImageWidth(), p.image->getImageHeight(), renderer->getTexture(p.image), 0.f, 1.f, std::nullopt, std::nullopt, FilterMode::LINEAR);
		renderer->drawText(identity.name.c_str(), pos.x, pos.y - 100.f, FontSize::NORMAL, 0.f);
		});
	renderer->render(hud.get());
	renderer->submit();
}

void Game::resize(int width, int height) {
	render();
}

```
