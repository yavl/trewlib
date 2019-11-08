#include "pm/pm.hpp"
#include "HelloWorld.hpp"
#include "pm/WindowManager.hpp"
#include "pm/FileHandle.hpp"
#include "pm/Logger.hpp"
#include <nlohmann/json.hpp>

using namespace pm;
using namespace nlohmann;

int main() {
	auto file = FileHandle("properties.json").asString();
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

		glClearColor(0.f, 0.5f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		m->render((float) dt);
		glfwSwapBuffers(window->getGlfwWindow());
		glfwPollEvents();
	}
	glfwTerminate();
}
