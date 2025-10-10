#include "DeprecatedHelloOpenGL.hpp"
#include <trew/drawables/impl_opengl/GLTexture.hpp>
#include <trew/nodes/Sprite.hpp>
#include <trew/Camera.hpp>
#include <trew/Shader.hpp>
#include <trew/InputManager.hpp>
#include <trew/AssetManager.hpp>
#include <trew/Logger.hpp>
#include <trew/actions/MoveAction.hpp>
#include <trew/drawables/impl_opengl/GLText.hpp>
#include <trew/scripting/ASManager.hpp>
#include <rapidcsv.h>
#include <trew/Color.hpp>
#include <trew/Globals.hpp>
#include <chrono>
#include <random>
#include <SDL3/SDL.h>
#include <trew/app/Window.hpp>
#include <trew/Hud.hpp>

const char* assetsRootDirectory = "assets";

DeprecatedHelloOpenGL::DeprecatedHelloOpenGL(Window* window) {
	this->window = window;
	cam = std::make_shared<Camera>(window);
	input = std::make_unique<InputManager>(window);
}

DeprecatedHelloOpenGL::~DeprecatedHelloOpenGL() {}

void DeprecatedHelloOpenGL::create() {
	assets = std::make_shared<AssetManager>(assetsRootDirectory);
	assets->load("default", AssetType::SHADER);
	assets->load("text", AssetType::SHADER);

	/*ASManager as(assets);
	as.registerScript("assets/scripts/main.as");
	as.runScript("assets/scripts/main.as");*/

	auto tex = assets->getTexture("tex.png");
	sprite = std::make_unique<Sprite>(tex);

	auto tex2 = assets->getTexture("tex2.png");
	auto sprite2 = new Sprite(tex2);
	sprite2->setXY(400, 300);
	sprite->addChild(sprite2);
	sprite->setRotation(-45.f);
	//sprite->addAction(new MoveAction(0.f, 200.f, 2.f));

	window->addWindowResizeCallback([this](int width, int height) {
		resize(width, height);
	});

	auto textSh = assets->getShader("text");
	text = std::make_unique<Text>(textSh, cam.get());
	
	rapidcsv::Document doc("assets/colhdr.csv");
	std::vector<float> col = doc.GetColumn<float>("Close");
	for (auto& each : col) {
		fmt::print("{}\n", each);
		texts.push_back(std::to_string(each));
	}

	auto circleTex = assets->getTexture("circle.png");

	{
		auto circleSprite = std::make_unique<Sprite>(circleTex);
		circleSprite->setXY(-1000, 0);
		circleSprite->setColor(Color(0, 0, 1, 1));
		//Globals::sprites.push_back(std::move(circleSprite));
	}

	{
		auto circleSprite = std::make_unique<Sprite>(circleTex);
		circleSprite->setXY(-800, 0);
		circleSprite->setColor(Color(1, 1, 0, 1));
		//Globals::sprites.push_back(std::move(circleSprite));
	}

	{
		auto circleSprite = std::make_unique<Sprite>(circleTex);
		circleSprite->setXY(-600, 0);
		//circleSprite->setColor(Globals::sprites[0].get() ->getColor() + Globals::sprites[1].get()->getColor());
		//Globals::sprites.push_back(std::move(circleSprite));
	}

	{
		std::mt19937_64 rng;
		// initialize the random number generator with time-dependent seed
		uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
		rng.seed(ss);
		// initialize a uniform distribution between 0 and 1
		std::uniform_real_distribution<double> unif(0, 1);
		// ready to generate random numbers
		for (int i = 0; i < 100; ++i) {
			auto circleSprite = std::make_unique<Sprite>(circleTex);
			circleSprite->setXY(100 + i * 100, 0);
			float r = unif(rng);
			float g = unif(rng);
			float b = unif(rng);
			circleSprite->setColor(Color(r, g, b, 1));
			//Globals::sprites.push_back(std::move(circleSprite));
		}
	}

	window->addMouseButtonCallback([=](int button) {
		if (button == SDL_BUTTON_RIGHT) {
			auto cursorPos = window->getCursorPos();
			auto world = cam->screenToSpace(cursorPos.x, cursorPos.y);
			auto moveAction = new MoveAction(world.x, world.y, 1.f);
			//Globals::sprites[0]->addAction(moveAction);
			log("mouseClick", fmt::format("World pos: {}, {}", world.x, world.y));
		}
	});

	hud = std::make_unique<Hud>(window);
}

void DeprecatedHelloOpenGL::update(float dt) {
	SDL_Event event;
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
	input->update();
	cam->update(dt);
	sprite->act(dt);
	//for (const auto& sprite : Globals::sprites) {
	//	sprite.get()->act(dt);
	//}
	//fmt::println("fps: {}", 1 / dt);
}

void DeprecatedHelloOpenGL::render() {
	glClearColor(0.f, 0.5f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	sprite->draw();
	//for (const auto& sprite : Globals::sprites) {
	//	sprite.get()->draw();
	//}

	float offset = 100.f;
	float prevY = 200.f;
	for (auto& textStr : texts) {
		text->draw(textStr, -1000.f, prevY + offset, 1.f, glm::vec3(1.f, 1.f, 1.f));
		prevY += offset;
	}
	//hud.get()->render();
	window->swapBuffers();
}

void DeprecatedHelloOpenGL::resize(int width, int height) {
	glViewport(0, 0, width, height);
	render();
}
