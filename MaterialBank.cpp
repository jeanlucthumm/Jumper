//
// Created by Jean-Luc Thumm on 12/10/17.
//

#include "MaterialBank.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

MaterialBank *MaterialBank::instance;

MaterialBank * MaterialBank::I() {
    return instance;
}

MaterialBank::MaterialBank() {
    // insert default material
    auto *material = new Material;
    material->ka = glm::vec3{0.19125, 0.0735, 0.0225};
    material->kd = glm::vec3{0.780392, 0.568627, 0.113725};
    material->ks = glm::vec3{0.25677, 0.137622, 0.086014};
    material->shiny = 0.1;
    table["default"] = std::shared_ptr<Material>{material};
}

bool MaterialBank::parse(const std::string &path) {
    std::ifstream in{path};
    if (!in.is_open()) {
        std::cerr << "Could not open file: " << path
                  << ": " << std::strerror(errno) << std::endl;
        return false;
    }

    std::string line;
    std::string token;
    float value, x, y, z;
    std::shared_ptr<Material> current = nullptr;

    while (std::getline(in, line)) {
        std::stringstream ss{line};
        ss >> token;

        if (current == nullptr) {
            if (token == "newmtl") {
                current = std::make_shared<Material>();
                ss >> token;
                current->name = token;
            }
            continue;
        }

        if (token == "Ns") {
            ss >> value;
            current->shiny = value;
        }
        else if (token == "Ka") {
            ss >> x;
            ss >> y;
            ss >> z;
            current->ka = glm::vec3{x, y, z};
        }
        else if (token == "Kd") {
            ss >> x;
            ss >> y;
            ss >> z;
            current->kd = glm::vec3{x, y, z};
        }
        else if (token == "Ks") {
            ss >> x;
            ss >> y;
            ss >> z;
            current->ks = glm::vec3{x, y, z};
        }
        else if (token == "map_Ka") {
            ss >> token;
            if (!(current->kaMap = putTexture(token))) {
                continue; // id of 0 is not possible
            }
            current->hasKaMap = true;
            current->kaMapPath = token;
        }
        else if (token == "map_Kd") {
            ss >> token;
            if (!(current->kdMap = putTexture(token))) {
                continue; // id of 0 is not possible
            }
            current->hasKdMap = true;
            current->kdMapPath = token;
        }
        else if (token == "newmtl") {
            table[current->name] = std::move(current);
            current = std::make_shared<Material>();

            ss >> token;
            current->name = token;
        }
    }

    // last one isn't pushed
    if (current != nullptr) {
        table[current->name] = std::move(current);
    }
    return true;
}

const std::shared_ptr<const Material> &MaterialBank::get(const std::string &name) const {
    auto itr = table.find(name);
    if (itr == table.end()) {
        return table.find("default")->second;
    }
    return itr->second;
}

bool MaterialBank::store(Material material) {
    if (table.find(material.name) != table.end()) {
        return false;
    }
    std::string name = material.name;
    table[name] = std::make_shared<Material>(std::move(material));
    return true;
    // TODO load textures if they have not been loaded
}

GLuint MaterialBank::putTexture(const std::string &path) {
    return 5;
}

void MaterialBank::instantiate() {
    if (instance) free();
    instance = new MaterialBank();
}

void MaterialBank::free() {
    delete instance;
}
