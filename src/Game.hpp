#pragma once

#include <trew/app/Application.hpp>
#include <memory>
#include <flecs.h>
#include <ecs/components/MotionComponents.hpp>
#include <ecs/components/IdentityComponents.hpp>
#include <ecs/components/RenderComponents.hpp>

namespace trew {
	class Camera;
	class InputManager;
	class AssetManager;
    class BaseUI;
	class Renderer;
	class Window;
	class Sprite;
	class Server;
	class Client;
}

using namespace trew;
using namespace game::components;

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
	flecs::query<Image, MapIdentity> mapQuery;
	flecs::query<Position, Image, CharacterIdentity> characterQuery;

	Window* window;
	std::unique_ptr<AssetManager> assets;
	std::unique_ptr<Camera> cam;
	std::unique_ptr<InputManager> input;
    std::unique_ptr<BaseUI> hud;
	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<Server> server;
	std::unique_ptr<Client> client;
};
