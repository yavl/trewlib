#include "pm.h"
#include <fmt/format.h>

namespace pm {
	Texture init_texture(const char* path) {
		Texture tex;
		tex.set_name(path);
		tex.texture = SOIL_load_OGL_texture(
			path,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_DDS_LOAD_DIRECT | SOIL_FLAG_INVERT_Y
		);
		if (!tex.texture) {
			fmt::print("SOIL loading error: '{}'\n", SOIL_last_result());
		}
		else {
			tex.texture = 1;
			fmt::print("[LOADED] Texture <{}>\n", tex.get_name());
		}

		int width, height;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
		tex.set_x(width);
		tex.set_y(height);
		return tex;
	}

	void draw_texture(Texture tex, GLfloat x, GLfloat y, GLuint sizeX, GLuint sizeY) {
		glBindTexture(GL_TEXTURE_2D, tex.texture);
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
}