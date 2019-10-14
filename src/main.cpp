#include "pm/pm.hpp"
#include "pm/Texture.hpp"
#include "pm/Shader.hpp"
#include "pm/InputManager.hpp"
#include "HelloWorld.hpp"
#include "pm/WindowManager.hpp"
#include <nlohmann/json.hpp>
#include "pm/FileHandle.hpp"
#include "pm/Logger.hpp"
#include <nlohmann/json.hpp>

using namespace pm;
using namespace nlohmann;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
HelloWorld* m;

int main() {
	json j;
	j["window"]["width"] = 800;
	j["window"]["height"] = 600;
	FileHandle("settings.json").writeString(j.dump(4));
	auto file = FileHandle("settings.json").asString();
	log("asd", file);
	auto window = std::make_unique<WindowManager>();
	window->createWindow("asd", 800, 600);

	m = new HelloWorld(window.get());
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
	delete m;
	m = nullptr;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	m->resize(width, height);
}
