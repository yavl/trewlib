#pragma once

#include <trew/app/Application.hpp>
#include <memory>
#include <vector>
#include <string>
#include <flecs.h>
#include <ecs/Components.hpp>

namespace trew {
	class Camera;
	class InputManager;
	class AssetManager;
    class Hud;
	class Renderer;
	class Window;
	class Sprite;
}

using namespace trew;

class Game : public Application {
public:
	Game(Window* window);
	virtual ~Game();
	void create() override;
	void update(float dt) override;
	void render() override;
	void resize(int width, int height) override;
private:
	flecs::world world;
	flecs::query<Position, Image> query;

	Window* window;
	std::unique_ptr<AssetManager> assets;
	std::unique_ptr<Camera> cam;
	std::unique_ptr<InputManager> input;
    std::unique_ptr<Hud> hud;
	std::unique_ptr<Renderer> renderer;
};
