#include "HelloWorld.hpp"
#include "pm/Texture.hpp"
#include "pm/Sprite.hpp"
#include "pm/Hud.hpp"
#include "pm/Camera.hpp"
#include "pm/Shader.hpp"
#include "pm/InputManager.hpp"
#include "pm/WindowManager.hpp"

HelloWorld::HelloWorld(WindowManager* window) {
	this->window = window;
	cam = std::make_unique<Camera>(window, window->getWidth(), window->getHeight());
	input = std::make_unique<InputManager>(window);
}

void HelloWorld::create() {
	Shader* sh = new Shader("default.vert", "default.frag");

	tex = new Texture("tex.png", sh, cam.get());
	sprite = std::make_unique<Sprite>(tex);
}

void HelloWorld::render(float dt) {
	cam->update(dt);
	input->update();
	sprite->draw();
}

void HelloWorld::resize(int width, int height) {
}
