#include "HelloWorld.hpp"
#include <trew/drawables/impl_glfw/Texture.hpp>
#include <trew/nodes/Sprite.hpp>
#include <trew/Camera.hpp>
#include <trew/Shader.hpp>
#include <trew/InputManager.hpp>
#include <trew/app/impl_glfw/GlfwWindow.hpp>
#include <trew/AssetManager.hpp>
#include <trew/Logger.hpp>
#include <trew/actions/MoveAction.hpp>
#include <trew/drawables/impl_glfw/Text.hpp>

HelloWorld::HelloWorld(std::weak_ptr<Window> window) {
	this->window = window;
	cam = std::make_unique<Camera>(window);
	input = std::make_unique<InputManager>(window);
}

HelloWorld::~HelloWorld() {}

void HelloWorld::create() {
	assets = std::make_unique<AssetManager>("assets/assets.json");
	assets->load("assets/default", AssetType::SHADER);
	assets->load("assets/text", AssetType::SHADER);
	assets->load("assets/tex.png", AssetType::TEXTURE);
	assets->load("assets/tex2.png", AssetType::TEXTURE);

	auto sh = assets->getShader("assets/default").value();
	auto tex = assets->getTexture("assets/tex.png").value();
	tex->setShader(sh);
	tex->setCamera(cam.get());
	sprite = std::make_unique<Sprite>(tex);

	auto tex2 = assets->getTexture("assets/tex2.png").value();
	tex2->setShader(sh);
	tex2->setCamera(cam.get());
	auto sprite2 = new Sprite(tex2);
	sprite2->setXY(400, 300);
	sprite->addChild(sprite2);
	//sprite->addAction(new MoveAction(0.f, 200.f, 2.f));

	window.lock()->addResizeCallback([this](int width, int height) {
		resize(width, height);
	});

	auto textSh = assets->getShader("assets/text").value();
	text = std::make_unique<Text>(textSh, cam.get());
}

void HelloWorld::update(float dt) {
	input->update();
	cam->update(dt);
	sprite->act(dt);
}

void HelloWorld::render() {
	glClearColor(0.f, 0.5f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	sprite->draw();
	text->draw("Example freetype text", 0.f, 600.f, 1.f, glm::vec3(0.5, 0.8f, 0.2f));
	window.lock()->swapBuffersPollEvents();
}

void HelloWorld::resize(int width, int height) {
	glViewport(0, 0, width, height);
}
