#include <ft2build.h>
#include FT_FREETYPE_H
#include <trew/trew.hpp>
#include <map>
#include <string>

namespace trew {
	class Shader;
	class Camera;
	struct Character {
		unsigned int TextureID; // ID handle of the glyph texture
		glm::ivec2   Size;      // Size of glyph
		glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
		unsigned int Advance;   // Horizontal offset to advance to next glyph
	};

	class Text {
	public:
		Text(Shader* shader, Camera* cam);
		virtual ~Text() = default;
		virtual void draw(std::string text, float x, float y, float scale, glm::vec3 color);
	private:
		GLuint VAO;
		GLuint VBO;
		std::map<GLchar, Character> Characters;
		Shader* shader;
		Camera* cam;
	};
}
