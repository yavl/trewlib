#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <fmt/format.h>
#include <OpenGL/glu.h>

using namespace std;

namespace pm {
	class Color {
	public:
		GLfloat r, g, b, a;
		Color() {
			r = 1.0f;
			g = 1.0f;
			b = 1.0f;
			a = 1.0f;
		}
		Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}
	};
	
	class Texture {
	private:
		GLint tex_width, tex_height;
		Color color;
		string name;
		const char* path;
	public:
		GLuint texture;
		
		GLint get_x() {
			return tex_width;
		}
		void set_x(GLint tex_width) {
			this->tex_width = tex_width;
		}
		
		GLint get_y() {
			return tex_height;
		}
		void set_y(GLint tex_height) {
			this->tex_height = tex_height;
		}
		
		void set_color(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
			this->color = Color(r, g, b, a);
		}
		
		string get_name() {
			return name;
		}
		void set_name(string name) {
			this->name = name;
		}
		const char* get_path() {
			return path;
		}
		
		void operator=(const GLuint& tex) {
			texture = tex;
		}
		
		void print_info() {
			fmt::print("[INFO] Texture name (id {}): {} ({} x {})\n", texture, name, tex_width, tex_height);
		}
		
		Texture() {}
		
		Texture(const char* path) {
			this->set_name(path);
			this->path = path;
			glGenTextures(0, &texture);
			glBindTexture(GL_TEXTURE_2D, this->texture);
			this->texture = SOIL_load_OGL_texture(
				path,
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_DDS_LOAD_DIRECT | SOIL_FLAG_INVERT_Y
			);
			if (!this->texture) {
				fmt::print("[ERROR] SOIL loading error: '{}'\n", SOIL_last_result());
			}
			else {
				fmt::print("[LOADED] Texture <{}>\n", this->get_name());
			}
			
			int width, height;
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
			this->set_x(width);
			this->set_y(height);
		}
		
		void draw_texture(GLfloat x, GLfloat y, GLuint sizeX, GLuint sizeY) {
			glBindTexture(GL_TEXTURE_2D, this->texture);
			glColor4f(color.r, color.g, color.b, color.a);
			glPushMatrix();
			glTranslatef(x, y, 0);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex2f(0.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex2f(0.0f, sizeY);
			glTexCoord2f(1.0f, 1.0f);
			glVertex2f(sizeX, sizeY);
			glTexCoord2f(1.0f, 0.0f);
			glVertex2f(sizeX, 0.0f);
			glEnd();
			glPopMatrix();
		}
		
		~Texture() {
			glDeleteTextures(1, &texture);
			texture = NULL;
			fmt::print("[DESTRUCTED] Texture <{}>\n", this->get_name());
		}
	};
	
	class Sprite {
	private:
		GLfloat x, y;
		Texture* tex;
	public:
		Sprite() {}
		
		Sprite(Texture* tex) {
			this->tex = tex;
		}
		
		Texture* get_texture() {
			return tex;
		}
		
		GLfloat get_pos_x() {
			return x;
		}
		void set_pos_x(GLfloat x) {
			this->x = x;
		}
		
		GLfloat get_pos_y() {
			return y;
		}
		void set_pos_y(GLfloat y) {
			this->y = y;
		}
		
		void set_pos(GLfloat x, GLfloat y) {
			this->x = x;
		}
		
		void draw() {
			tex->draw_texture(x, y, tex->get_x(), tex->get_y());
		}
	};
	
	class Camera {
	private:
		GLfloat x, y;
		const GLfloat cam_speed = 0.02f;
	public:
		Camera() {}
		GLfloat get_x() {
			return x;
		}
		void set_x(GLfloat x) {
			this->x = x;
		}
		
		GLfloat get_y() {
			return y;
		}
		void set_y(GLfloat y) {
			this->y = y;
		}
		
		void set_pos(GLfloat x, GLfloat y) {
			this->x = x;
			this->y = y;
		}
		
		void update(GLFWwindow* window) {
			int state = glfwGetKey(window, GLFW_KEY_W);
			if (state == GLFW_PRESS) {
				set_y(get_y() - cam_speed);
			}
			
			state = glfwGetKey(window, GLFW_KEY_S);
			if (state == GLFW_PRESS) {
				set_y(get_y() + cam_speed);
			}
			
			state = glfwGetKey(window, GLFW_KEY_A);
			if (state == GLFW_PRESS) {
				set_x(get_x() - cam_speed);
			}
			
			state = glfwGetKey(window, GLFW_KEY_D);
			if (state == GLFW_PRESS) {
				set_x(get_x() + cam_speed);
			}
			
			gluOrtho2D(x-1.0, x+1.0, y-1.0,y+1.0);
		}
		
		~Camera() {
			fmt::print("[DESTRUCTED] Camera\n");
		}
	};
}

