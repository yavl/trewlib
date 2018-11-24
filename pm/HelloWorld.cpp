#include "HelloWorld.hpp"

Main::Main(GLFWwindow* window, int width, int height) {
	this->window = window;
	cam = Camera(window, width, height);
	input = InputManager(window);
}

void Main::create() {
	Shader* sh = new Shader("default.vert", "default.frag");

	tex = new Texture("tex.png", sh, &cam);
	tex2 = new Texture("tex2.png", sh, &cam);

	sprite = Sprite(tex);
	sprite2 = Sprite(tex2);

	hud = Hud(window);
}

void Main::render(float dt) {
	cam.update(dt);
	input.update();
	sprite.draw();
	sprite2.draw();
	hud.display();
}

void Main::resize(int width, int height) {
	cam.update_window_data(width, height);
}

Main::~Main() {
	delete tex;
	delete tex2;
	delete sh;
}