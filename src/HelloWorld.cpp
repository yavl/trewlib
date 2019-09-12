#include "HelloWorld.hpp"
#include "pm/Texture.hpp"
#include "pm/Sprite.hpp"
#include "pm/Hud.hpp"
#include "pm/Camera.hpp"
#include "pm/Shader.hpp"
#include "pm/InputManager.hpp"
#include "pm/WindowManager.hpp"

Main::Main(WindowManager* window) {
	this->window = window;
	cam = std::make_unique<Camera>(window, window->getWidth(), window->getHeight());
	input = std::make_unique<InputManager>(window);
}

void Main::create() {
	Shader* sh = new Shader("default.vert", "default.frag");

	tex = new Texture("tex.png", sh, cam.get());
	tex2 = new Texture("tex2.png", sh, cam.get());

	sprite = std::make_unique<Sprite>(tex);
	sprite2 = std::make_unique<Sprite>(tex2);
    hud = std::make_unique<Hud>(window->getGlfwWindow());
}

void Main::render(float dt) {
	cam->update(dt);
	input->update();
	sprite->draw();
	sprite2->draw();
    hud->display();
}

void Main::resize(int width, int height) {
	cam->updateWindowData(width, height);
}

Main::~Main() {
}
