#pragma once

#include <trew/app/Application.hpp>
#include <memory>
#include <vector>
#include <string>

namespace trew {
	class Camera;
	class InputManager;
	class Window;
	class AssetManager;
}

using namespace trew;

class HelloSDL3GPU : public Application {
public:
	HelloSDL3GPU(std::weak_ptr<Window> window);
	virtual ~HelloSDL3GPU();
	void create() override;
	void update(float dt) override;
	void render() override;
	void resize(int width, int height) override;
private:
	std::weak_ptr<Window> window;
	std::shared_ptr<AssetManager> assets;
	std::shared_ptr<Camera> cam;
	std::unique_ptr<InputManager> input;
};
