#pragma once

#include <memory>

class asIScriptEngine;

namespace trew {
	class AssetManager;
	class ASManager {
	public:
		ASManager(std::shared_ptr<AssetManager> assets);
		void registerScript(const char* path);
		void runScript(const char* path);
	private:
		asIScriptEngine* engine;
		std::weak_ptr<AssetManager> assets;
	};
}