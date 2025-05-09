#include "HelloSDL3GPU.hpp"
#include "HelloWorld.hpp"
#include <trew/app/SdlWindow.hpp>
#include <trew/FileHandle.hpp>
#include <trew/Logger.hpp>
#include <nlohmann/json.hpp>
#include <SDL3/SDL.h>

using namespace trew;
using namespace nlohmann;

int main() {
	Logger::getInstance().setLogLevel(LogLevel::LOG_DEBUG);

	auto file = FileHandle("assets/properties.json").asString();
	json j = json::parse(file);
	auto windowProperties = j["window"];
	int width = windowProperties["width"].get<int>();
	int height = windowProperties["height"].get<int>();
	std::string title = windowProperties["title"].get<std::string>();

	auto window = std::make_shared<SdlWindow>();
	window->createWindow(title, width, height);

	auto m = std::make_unique<HelloWorld>(window);
	//auto m = std::make_unique<HelloSDL3GPU>(window);
	m->create();
	Uint64 now = SDL_GetPerformanceCounter();
	Uint64 last = 0;
	float dt = 0;
	while (!window->shouldClose()) {
		last = now;
		now = SDL_GetPerformanceCounter();
		dt = static_cast<float>((now - last) / static_cast<float>(SDL_GetPerformanceFrequency()));
		m->update(static_cast<float>(dt));
		m->render();
	}
}
