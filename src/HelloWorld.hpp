#pragma once

#include "pm/Texture.hpp"
#include "pm/Sprite.hpp"
#include "pm/Shader.hpp"
#include "pm/InputManager.hpp"
#include "pm/Hud.hpp"
#include "pm/Camera.hpp"

using namespace pm;

class Main {
private:
	Camera cam;
	InputManager input;
	Texture* tex;
	Texture* tex2;
	Sprite sprite;
	Sprite sprite2;
	GLFWwindow* window;
	Hud hud;
	Shader* sh;
public:
	Main(GLFWwindow* window, int width, int height);
	~Main();
	void create();
	void render(float dt);
	void resize(int width, int height);
};