#pragma once

class asIScriptEngine;

namespace trew {
	class ASManager {
	public:
		ASManager();
		void registerScript(const char* path);
		void runScript(const char* path);
	private:
		asIScriptEngine* engine;
	};
}