#include "HelloWorld.hpp"

Main::Main(GLFWwindow* window, int width, int height) {
	this->window = window;
	cam = Camera(window, width, height);
	input = InputManager(window);
}

Main::~Main() {
	delete tex;
	delete tex2;
}

void Main::create() {
	Shader sh("default.vert", "default.frag");

	tex = new Texture("tex.png", sh.get_shader_program(), &cam);
	tex2 = new Texture("tex2.png", sh.get_shader_program(), &cam);

	sprite = Sprite(tex);
	sprite2 = Sprite(tex2);

	//text = new TextRenderer(&cam);
}

void Main::render(float dt) {
	cam.update(dt);
	input.update();
	sprite.draw();
	sprite2.draw();
	//text->display();
}

void Main::resize(int width, int height) {
	cam.update_window_data(width, height);
}