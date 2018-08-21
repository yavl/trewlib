# pm
very tiny cross-platform game library
```c++
#include "pm/Texture.hpp"
#include "pm/Sprite.hpp"
#include "pm/Shader.hpp"
#include "pm/InputManager.hpp"

using namespace pm;

class Main {
private:
	Camera cam;
	InputManager input;
	Texture* tex;
	Texture* tex2;
	Sprite sprite;
	Sprite sprite2;
	GLFWwindow* window;
public:
	Main(GLFWwindow* window, int width, int height) {
		this->window = window;
		cam = Camera(window, width, height);
		input = InputManager(window);
	}

	void create() {
		Shader sh("default.vert", "default.frag");

		tex = new Texture("tex.png", sh.get_shader_program(), &cam);
		tex2 = new Texture("tex2.png", sh.get_shader_program(), &cam);

		sprite = Sprite(tex);
		sprite2 = Sprite(tex2);
	}

	void render(float dt) {
		cam.update(dt);
		input.update();
		sprite.draw();
		sprite2.draw();
	}

	void resize(int width, int height) {
		cam.update_projection_matrix(width, height);
	}

	~Main() {
		delete tex;
		delete tex2;
	}
};
```
