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

class HelloWorld {
private:
	std::unique_ptr<Camera> cam;
	std::unique_ptr<InputManager> input;
	Texture* tex;
	std::unique_ptr<Sprite> sprite;
	WindowManager* window;
	Shader* sh;
public:
	HelloWorld(WindowManager* window);
	virtual ~HelloWorld() = default;
	void create();
	void render(float dt);
	void resize(int width, int height);
};