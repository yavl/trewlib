#include "HelloWorld.hpp"
#include "pm/Texture.hpp"
#include "pm/Sprite.hpp"
#include "pm/Hud.hpp"
#include "pm/Camera.hpp"
#include "pm/Shader.hpp"
#include "pm/InputManager.hpp"
#include "pm/WindowManager.hpp"
#include "pm/AssetManager.hpp"
#include "pm/Logger.hpp"

HelloWorld::HelloWorld(WindowManager* window) {
	this->window = window;
	cam = std::make_unique<Camera>(window, window->getWidth(), window->getHeight());
	input = std::make_unique<InputManager>(window);
}

void HelloWorld::create() {
	assets = new AssetManager("assets");
	assets->load("default", AssetType::SHADER);
	assets->load("tex.png", AssetType::TEXTURE);

	Shader* sh = assets->getShader("default").value();
	tex = assets->getTexture("tex.png").value();
	tex->setShader(sh);
	tex->setCamera(cam.get());
	sprite = std::make_unique<Sprite>(tex);

	logDebug("hwasas", "asd");
	logError("qweqwee", "aswed");
}

void HelloWorld::render(float dt) {
	cam->update(dt);
	input->update();
	sprite->draw();
}

void HelloWorld::resize(int width, int height) {
}
