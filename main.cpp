#define GLFW_INCLUDE_GLEXT

#include "Window.h"
#include "MaterialBank.hpp"

#include "debug.h"

int main() {
//    try {
//        Window window{640, 480};
//        window.start();
//    } catch (std::runtime_error &error) {
//        std::cerr << error.what() << std::endl;
//        exit(EXIT_FAILURE);
//    }
    MaterialBank::instantiate();
    MaterialBank::I()->parse("obj/porsche/mod.mtl");

    auto material = MaterialBank::I()->get("02___Default");

    cout << material->name << endl;
    cout << glm::to_string(material->ka) << endl;
    cout << glm::to_string(material->kd) << endl;
    cout << glm::to_string(material->ks) << endl;
    cout << material->shiny << endl;
    cout << "ka id: " << material->kaMap << endl;
    cout << "kd id: " << material->kdMap << endl;
    cout << "has ka map: " << material->hasKaMap << endl;
    cout << "has kd map: " << material->hasKdMap << endl;

    MaterialBank::free();
    exit(EXIT_SUCCESS);
}