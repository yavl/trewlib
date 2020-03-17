#include "trew/trew.hpp"
#include "HelloWorld.hpp"
#include "trew/WindowManager.hpp"
#include "trew/FileHandle.hpp"
#include "trew/Logger.hpp"
#include <nlohmann/json.hpp>

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

	auto window = std::make_shared<WindowManager>();
	window->createWindow(title, width, height);

	auto m = std::make_unique<HelloWorld>(window);
	m->create();
	double lastFrame = 0.0;
	double currentFrame, dt;
	int frameCount = 0;
	double prevTime = glfwGetTime();
	while (!window->shouldClose()) {
		currentFrame = glfwGetTime();
		dt = currentFrame - lastFrame;
		lastFrame = currentFrame;
		frameCount++;
		if (currentFrame - prevTime >= 1.0f) {
			//fmt::print("{} fps\n", frameCount);
			frameCount = 0;
			prevTime = currentFrame;
		}

		m->update(static_cast<float>(dt));
		m->render();
	}
}
