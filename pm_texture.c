#include "pm.h"

Texture init_texture(const char* path) {
	Texture tex;
	SOIL_load_OGL_texture(
		path,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_DDS_LOAD_DIRECT | SOIL_FLAG_INVERT_Y
	);
	if (!tex.texture) {
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	} else tex.texture = 1;
	
	int width, height;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	tex.x = width;
	tex.y = height;
	return tex;
}