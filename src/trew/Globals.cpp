#include "Globals.hpp"

#include <trew/nodes/Sprite.hpp>

namespace trew::Globals {
	std::shared_ptr<trew::AssetManager> assets;
	std::shared_ptr<trew::Camera> camera;
	std::vector<std::unique_ptr<Sprite>> sprites;
}