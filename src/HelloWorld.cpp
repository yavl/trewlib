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
#include <trew/scripting/ASManager.hpp>
#include <rapidcsv.h>

const char* assetsRootDirectory = "assets";

HelloWorld::HelloWorld(std::weak_ptr<Window> window) {
	this->window = window;
	cam = std::make_unique<Camera>(window);
	input = std::make_unique<InputManager>(window);
}

HelloWorld::~HelloWorld() {}

void HelloWorld::create() {
	assets = std::make_shared<AssetManager>(assetsRootDirectory);
	assets->load("default", AssetType::SHADER);
	assets->load("text", AssetType::SHADER);
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
	//sprite->addAction(new MoveAction(0.f, 200.f, 2.f));

	window.lock()->addResizeCallback([this](int width, int height) {
		resize(width, height);
	});

	auto textSh = assets->getShader("text").value();
	text = std::make_unique<Text>(textSh, cam.get());
	
	rapidcsv::Document doc("assets/colhdr.csv");
	std::vector<float> col = doc.GetColumn<float>("Close");
	for (auto& each : col) {
		fmt::print("{}\n", each);
		texts.push_back(std::to_string(each));
	}

	ASManager as(assets);
	as.registerScript("assets/scripts/main.as");
	as.runScript("assets/scripts/main.as");
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

	float offset = 100.f;
	float prevY = 200.f;
	for (auto& textStr : texts) {
		text->draw(textStr, -1000.f, prevY + offset, 1.f, glm::vec3(1.f, 1.f, 1.f));
		prevY += offset;
	}
	window.lock()->swapBuffersPollEvents();
}

void HelloWorld::resize(int width, int height) {
	glViewport(0, 0, width, height);
	render();
}
