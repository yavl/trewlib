#pragma once

#include <memory>

namespace trew {
	class AssetManager;
	class Camera;
	namespace Globals {
		extern std::shared_ptr<trew::AssetManager> assets;
		extern std::shared_ptr<trew::Camera> camera;
	}
}
