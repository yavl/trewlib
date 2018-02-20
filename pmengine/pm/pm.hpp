//
//  pm.hpp
//  pmengine
//
//  Created by vlad on 21.02.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

#ifndef pm_hpp
#define pm_hpp

#include <GLFW/glfw3.h>
#include <OpenGL/glu.h>
#include <SOIL/SOIL.h>
#include <fmt/format.h>
#include <string>

namespace pm {
    class Color {
    public:
        GLfloat r, g, b, a;
        Color();
        Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
    };
    
    class Texture {
    private:
        GLint tex_width, tex_height;
        Color color;
        std::string name;
        const char* path;
    public:
        GLuint texture;
        
        GLint get_x();
        void set_x(GLint tex_width);
        
        GLint get_y();
        void set_y(GLint tex_height);
        
        void set_color(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
        Color get_color();
        
        std::string get_name();
        void set_name(std::string name);
        const char* get_path();
        
        void operator=(const GLuint& tex);
        
        void print_info();
        
        Texture();
        
        Texture(const char* path);
        
        void draw_texture(GLfloat x, GLfloat y, GLuint sizeX, GLuint sizeY);
        
        ~Texture();
    };
    
    class Sprite {
    private:
        GLfloat x, y;
        Texture* tex;
        Color color;
    public:
        Sprite();
        
        Sprite(Texture* tex);
        
        Texture* get_texture();
        
        GLfloat get_pos_x();
        void set_pos_x(GLfloat x);
        
        GLfloat get_pos_y();
        void set_pos_y(GLfloat y);
        
        void set_pos(GLfloat x, GLfloat y);
        
        void set_color(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
        
        void draw();
    };
    
    class Camera {
    private:
        GLfloat x, y;
        const GLfloat cam_speed = 0.02f;
    public:
        Camera();
        GLfloat get_x();
        void set_x(GLfloat x);
        
        GLfloat get_y();
        void set_y(GLfloat y);
        
        void set_pos(GLfloat x, GLfloat y);
        
        void update(GLFWwindow* window);
        
        ~Camera();
    };
}

#endif /* pm_hpp */


