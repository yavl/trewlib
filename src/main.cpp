#include "pm/pm.hpp"
#include "pm/Texture.hpp"
#include "pm/Shader.hpp"
#include "pm/InputManager.hpp"
#include "HelloWorld.hpp"
#include "pm/WindowManager.hpp"

using namespace pm;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
HelloWorld* m;

int main() {
	auto window = std::make_unique<WindowManager>();
	window->createWindow("hello", 800, 600);

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
