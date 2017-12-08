#define GLFW_INCLUDE_GLEXT

#include "Window.h"

int main() {
    Window window{640, 480};
    window.start();

    exit(EXIT_SUCCESS);
}