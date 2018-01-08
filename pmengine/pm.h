#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <fmt/format.h>

using namespace std;

namespace pm {
	class Texture {
	private:
		GLint x, y;
		string name;
	public:
		GLuint texture;

		GLint get_x() {
			return x;
		}
		void set_x(GLint x) {
			this->x = x;
		}

		GLint get_y() {
			return y;
		}
		void set_y(GLint y) {
			this->y = y;
		}

		string get_name() {
			return name;
		}
		void set_name(string name) {
			this->name = name;
		}

		Texture& operator=(const GLuint& tex) {
			texture = tex;
		}

		void print_info() {
			fmt::print("Texture name: {}\nResolution: {} x {}\n", name, x, y);
		}
	};
	void render();
	void draw_texture(Texture tex, GLfloat, GLfloat, GLuint, GLuint);
	Texture init_texture(const char*);
}