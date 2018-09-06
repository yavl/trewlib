#pragma once

#include "pm.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

namespace pm {
	class TextRenderer {
	private:
		FT_Library ft;
		FT_Face face;
		FT_GlyphSlot g;
		GLuint shader_id;
		Camera* cam;
		GLuint attribute_coord;
		GLuint VBO;
		GLuint tex;
	public:
		TextRenderer(Camera* cam);
		~TextRenderer();
		void render_text(const char *text, float x, float y, float sx, float sy);
		void display();
	};
}