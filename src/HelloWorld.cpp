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

HelloWorld::HelloWorld(std::shared_ptr<WindowManager> window) {
	this->window = window;
	cam = std::make_unique<Camera>(window);
	input = std::make_unique<InputManager>(window);
}

HelloWorld::~HelloWorld() {}

void HelloWorld::create() {
	assets = std::make_unique<AssetManager>("assets");
	assets->load("default", AssetType::SHADER);
	assets->load("tex.png", AssetType::TEXTURE);
	assets->load("tex2.png", AssetType::TEXTURE);

	auto sh = assets->getShader("default").value();
	auto tex = assets->getTexture("tex.png").value();
	tex->setShader(sh);
	tex->setCamera(cam.get());
	sprite = std::make_unique<Sprite>(tex);

	auto tex2 = assets->getTexture("tex2.png").value();
	tex2->setShader(sh);
	tex2->setCamera(cam.get());
	sprite2 = std::make_unique<Sprite>(tex2);
	sprite2->setXY(400, 300);

	hud = std::make_unique<Hud>(window->getGlfwWindow());

	window->addFramebufferSizeCallback([this](int width, int height) {
		resize(width, height);
	});
}

void HelloWorld::render(float dt) {
	cam->update(dt);
	input->update();
	sprite->draw();
	sprite2->draw();
	hud->display();
}

void HelloWorld::resize(int width, int height) {
	glViewport(0, 0, width, height);
}
