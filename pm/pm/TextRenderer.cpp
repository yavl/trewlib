#include "TextRenderer.hpp"

using namespace pm;

TextRenderer::TextRenderer(Camera* cam) {
	this->cam = cam;

	if (FT_Init_FreeType(&ft))
		fprintf(stderr, "Could not init freetype library\n");
	if (FT_New_Face(ft, "Ubuntu-Regular.ttf", 0, &face))
		fprintf(stderr, "Could not open font\n");
	FT_Set_Pixel_Sizes(face, 0, 48);
	g = face->glyph;

	Shader fontsh("fontsh.vert", "fontsh.frag");
	shader_id = fontsh.get_shader_program();

	const char* attribute_name = "coord";
	attribute_coord = glGetAttribLocation(shader_id, attribute_name);
	if (attribute_coord == -1)
		fmt::print("[ERROR] Could not bind attribute: <{}>\n", attribute_name);

	glGenBuffers(1, &VBO);
	glEnableVertexAttribArray(attribute_coord);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

TextRenderer::~TextRenderer() {}

// yet non-working copypaste
void TextRenderer::render_text(const char *text, float x, float y, float sx, float sy) {
	const char *p;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	GLuint tex_id = glGetUniformLocation(shader_id, "u_texture");
	glUniform1i(tex_id, 0);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	/* Set up the VBO for our vertex data */
	glEnableVertexAttribArray(attribute_coord);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);

	for (p = text; *p; p++) {
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
			continue;

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			g->bitmap.width,
			g->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			g->bitmap.buffer
		);

		float x2 = x + g->bitmap_left * sx;
		float y2 = -y - g->bitmap_top * sy;
		float w = g->bitmap.width * sx;
		float h = g->bitmap.rows * sy;

		GLfloat box[4][4] = {
			{ x2,     -y2    , 0, 0 },
		{ x2 + w, -y2    , 1, 0 },
		{ x2,     -y2 - h, 0, 1 },
		{ x2 + w, -y2 - h, 1, 1 },
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		x += (g->advance.x / 64) * sx;
		y += (g->advance.y / 64) * sy;
	}

	glDisableVertexAttribArray(attribute_coord);
	glDeleteTextures(1, &tex);
}

// yet non-working copypaste
void TextRenderer::display() {
	glm::mat4 matrix = glm::mat4(1.0f);
	glUseProgram(shader_id);
	GLuint matrix_id = glGetUniformLocation(shader_id, "u_projTrans");
	glUniformMatrix4fv(matrix_id, 1, GL_FALSE, glm::value_ptr(cam->projection_matrix * matrix));
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	GLfloat black[4] = { 0, 0, 0, 1 };
	GLuint color_id = glGetUniformLocation(shader_id, "u_color");
	glUniform4fv(color_id, 1, black);

	float sx = 2.0f / 800.0f;
	float sy = 2.0f / 600.0f;

	render_text("The Quick Brown Fox Jumps Over The Lazy Dog",
		-1 + 8 * sx, 1 - 50 * sy, sx, sy);
	render_text("The Misaligned Fox Jumps Over The Lazy Dog",
		-1 + 8.5 * sx, 1 - 100.5 * sy, sx, sy);

}