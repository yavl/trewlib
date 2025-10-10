#pragma once

class asIScriptEngine;

namespace trew {
	class AssetManager;
	class Camera;
	class ASManager {
	public:
		ASManager(AssetManager* assets, Camera* cam);
		void registerScript(const char* path);
		void runScript(const char* path);
	private:
		asIScriptEngine* engine;
		AssetManager* assets;
		Camera* cam;
	};
}