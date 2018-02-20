#include "pm.hpp"

pm::Camera::Camera() {}

GLfloat pm::Camera::get_x() {
    return x;
}
void pm::Camera::set_x(GLfloat x) {
    this->x = x;
}

GLfloat pm::Camera::get_y() {
    return y;
}
void pm::Camera::set_y(GLfloat y) {
    this->y = y;
}

void pm::Camera::set_pos(GLfloat x, GLfloat y) {
    this->x = x;
    this->y = y;
}

void pm::Camera::update(GLFWwindow* window) {
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

pm::Camera::~Camera() {
    fmt::print("[DESTRUCTED] Camera\n");
}


