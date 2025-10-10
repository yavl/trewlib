#include "Globals.hpp"

#include <trew/nodes/Sprite.hpp>

namespace trew::Globals {
	AssetManager* assets;
	Camera* camera;
	std::vector<std::unique_ptr<Sprite>> sprites;
}