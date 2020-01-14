#pragma once

#include <memory>

namespace trew {
	class Texture;
	class Shader;
	class Camera;
	class Sprite;
	class InputManager;
	class Hud;
	class WindowManager;
	class AssetManager;
}

using namespace trew;

struct GLFWwindow;

class HelloWorld {
public:
	HelloWorld(std::shared_ptr<WindowManager> window);
	virtual ~HelloWorld();
	void create();
	void update(float dt);
	void render();
	void resize(int width, int height);
private:
	std::shared_ptr<WindowManager> window;
	std::unique_ptr<Camera> cam;
	std::unique_ptr<InputManager> input;
	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<Hud> hud;
	std::unique_ptr<AssetManager> assets;
};
