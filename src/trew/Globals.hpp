#pragma once

#include <memory>
#include <vector>

namespace trew {
	class AssetManager;
	class Camera;
	class Sprite;
	namespace Globals {
		extern std::shared_ptr<trew::AssetManager> assets;
		extern std::shared_ptr<trew::Camera> camera;
		extern std::vector<std::unique_ptr<Sprite>> sprites;
	}
}
