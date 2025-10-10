#pragma once

#include <memory>
#include <vector>

namespace trew {
	class AssetManager;
	class Camera;
	class Sprite;
	/// <summary>
	/// Globals are needed for AngelScript global functions
	/// </summary>
	namespace Globals {
		extern AssetManager* assets;
		extern Camera* camera;
		extern std::vector<std::unique_ptr<Sprite>> sprites;
	}
}
