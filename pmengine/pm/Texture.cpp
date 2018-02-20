#include "pm.hpp"

GLint pm::Texture::get_x() {
    return tex_width;
}
void pm::Texture::set_x(GLint tex_width) {
    this->tex_width = tex_width;
}

GLint pm::Texture::get_y() {
    return tex_height;
}
void pm::Texture::set_y(GLint tex_height) {
    this->tex_height = tex_height;
}

void pm::Texture::set_color(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    this->color = Color(r, g, b, a);
}

std::string pm::Texture::get_name() {
    return name;
}
void pm::Texture::set_name(std::string name) {
    this->name = name;
}
const char* pm::Texture::get_path() {
    return path;
}

void pm::Texture::operator=(const GLuint& tex) {
    texture = tex;
}

void pm::Texture::print_info() {
    fmt::print("[INFO] Texture name (id {}): {} ({} x {})\n", texture, name, tex_width, tex_height);
}

pm::Texture::Texture() {}

pm::Texture::Texture(const char* path) {
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

void pm::Texture::draw_texture(GLfloat x, GLfloat y, GLuint sizeX, GLuint sizeY) {
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

pm::Texture::~Texture() {
    glDeleteTextures(1, &texture);
    texture = NULL;
    fmt::print("[DESTRUCTED] Texture <{}>\n", this->get_name());
}


