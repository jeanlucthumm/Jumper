#include "util.h"
#include "GLinclude.h"
#include <iostream>
#include <glm/ext.hpp>

using std::cerr;
using std::endl;
using std::cout;

void util::print_error(GLenum error) {
    cerr << error_string(error) << endl;
}

std::string util::error_string(GLenum error) {
    switch (error) {
        case GL_NO_ERROR:
            return "GL_NO_ERROR";
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION";
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY";
        default:
            return "default";
            break;
    }
}

void util::before_error() {
    cerr << "Before:\t" << error_string(glGetError()) << endl;
}

void util::after_error() {
    cerr << "After:\t" << error_string(glGetError()) << endl;
}

glm::vec4 util::rgba_normalize(float r, float g, float b) {
    return glm::vec4 {
        r / 255.0f,
        g / 255.0f,
        b / 255.0f,
        1.0f
    };
}

void util::print(std::string_view name, const std::vector<glm::vec3> &vec) {
    cout << "----[ " << name << " ]----" << endl;
    for (auto &e : vec) {
        cout << glm::to_string(e) << endl;
    }
}

void util::print(std::string_view name, const std::vector<glm::vec2> &vec) {
    cout << "----[ " << name << " ]----" << endl;
    for (auto &e : vec) {
        cout << glm::to_string(e) << endl;
    }
}
