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
    auto material = MaterialBank::I()->get("myglass");

    cout << material->name << endl;
    cout << material->shiny << endl;
    cout << glm::to_string(material->kd) << endl;
    cout << glm::to_string(material->ks) << endl;
    cout << "has kd map: " << material->hasKdMap << endl;
    cout << "--------------" << endl;

    auto b = MaterialBank::I()->get("02___Default");

    cout << b->name << endl;
    cout << b->shiny << endl;
    cout << "has kd map: " << b->hasKdMap << endl;
    cout << b->kdMapPath << endl;
    cout << "--------------" << endl;

    auto def = MaterialBank::I()->get("asdfasdf");

    cout << def->name << endl;
    cout << def->shiny << endl;
    cout << glm::to_string(def->kd) << endl;

    MaterialBank::free();
    exit(EXIT_SUCCESS);
}