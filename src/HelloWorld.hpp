#pragma once

#include <memory>

namespace pm {
	class Texture;
	class Shader;
	class Camera;
	class Sprite;
	class InputManager;
	class Hud;
	class WindowManager;
}

using namespace pm;

struct GLFWwindow;

class Main {
private:
	std::unique_ptr<Camera> cam;
	std::unique_ptr<InputManager> input;
	Texture* tex;
	Texture* tex2;
	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<Sprite> sprite2;
	WindowManager* window;
	std::unique_ptr<Hud> hud;
	Shader* sh;
public:
	Main(WindowManager* window);
	~Main();
	void create();
	void render(float dt);
	void resize(int width, int height);
};