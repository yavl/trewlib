#include "HelloWorld.hpp"
#include "trew/Texture.hpp"
#include "trew/Sprite.hpp"
#include "trew/Hud.hpp"
#include "trew/Camera.hpp"
#include "trew/Shader.hpp"
#include "trew/InputManager.hpp"
#include "trew/WindowManager.hpp"
#include "trew/AssetManager.hpp"
#include "trew/Logger.hpp"
#include "trew/actions/MoveAction.hpp"

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
	auto sprite2 = new Sprite(tex2);
	sprite2->setXY(400, 300);
	sprite->addChild(sprite2);
	sprite->addAction(new MoveAction(2000, 1000, 2.f));

	hud = std::make_unique<Hud>(window->getGlfwWindow());

	window->addFramebufferSizeCallback([this](int width, int height) {
		resize(width, height);
	});
}

void HelloWorld::update(float dt) {
	cam->update(dt);
	sprite->act(dt);
}

void HelloWorld::render() {
	input->update();
	sprite->draw();
	hud->display();
}

void HelloWorld::resize(int width, int height) {
	glViewport(0, 0, width, height);
}