#define GLFW_INCLUDE_GLEXT

#include "Window.h"
#include "debug.h"
#include <iostream>

int main() {
    try {
        Window window{640, 480};
        window.start();
    } catch (std::runtime_error &error) {
        std::cerr << error.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
