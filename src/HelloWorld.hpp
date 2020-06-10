#pragma once

#include <trew/app/Application.hpp>
#include <memory>

namespace trew {
	class Texture;
	class Shader;
	class Camera;
	class Sprite;
	class InputManager;
	class Window;
	class AssetManager;
	class Text;
}

using namespace trew;

struct GLFWwindow;

class HelloWorld : public Application {
public:
	HelloWorld(std::weak_ptr<Window> window);
	virtual ~HelloWorld();
	void create() override;
	void update(float dt) override;
	void render() override;
	void resize(int width, int height) override;
private:
	std::weak_ptr<Window> window;
	std::unique_ptr<Camera> cam;
	std::unique_ptr<InputManager> input;
	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<AssetManager> assets;
	std::unique_ptr<Text> text;
};
