//
// Created by Jean-Luc Thumm on 12/10/17.
//

#include "MaterialBank.hpp"
#include "lib/stb_image.h"
#include "util.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <boost/filesystem.hpp>

#include "debug.h" // DEBUG

namespace fs = boost::filesystem;

MaterialBank *MaterialBank::instance;

MaterialBank *MaterialBank::I() {
    return instance;
}

MaterialBank::MaterialBank() {
    // insert default material
    auto *material = new Material;
    material->name = "default";
    material->ka = glm::vec3{0.19125, 0.0735, 0.0225};
    material->kd = glm::vec3{0.780392, 0.568627, 0.113725};
    material->ks = glm::vec3{0.25677, 0.137622, 0.086014};
    material->shiny = 0.1;
    table["default"] = std::shared_ptr<Material>{material};
}

bool MaterialBank::parse(const std::string &path) {
    cout << "Parsing: " << path << endl; // DEBUG
    std::ifstream in{path};
    if (!in.is_open()) {
        std::cerr << "Could not open file: " << path
                  << ": " << std::strerror(errno) << std::endl;
        return false;
    }
    fs::path bpath = fs::path{path}.parent_path();

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
            current->hasKaMap = true;
            current->kaMapPath = (bpath / fs::path{token}).string();
        }
        else if (token == "map_Kd") {
            ss >> token;
            current->hasKdMap = true;
            current->kdMapPath = (bpath / fs::path{token}).string();
        }
        else if (token == "newmtl") {
            // load textures of old material
            bool ok = true;
            if (current->hasKaMap) {
                current->kaMap = loadTexture(current->kaMapPath);
                if (!current->kaMap) ok = false;
            }
            if (current->hasKdMap) {
                current->kdMap = loadTexture(current->kdMapPath);
                if (!current->kdMap) ok = false;
            }

            if (ok) {
                std::cout << "Loaded material: " << current->name << std::endl;
                table[current->name] = std::move(current);
            }
            current = std::make_shared<Material>();

            ss >> token;
            current->name = token;
        }
    }

    // last one isn't pushed
    if (current != nullptr) {
        std::cout << "Loaded material: " << current->name << std::endl;
        table[current->name] = std::move(current);
    }

    return true;
}

const std::shared_ptr<const Material> &MaterialBank::get(const std::string &name) {
    auto itr = table.find(name);
    if (itr == table.end()) {
        std::cerr << "Could not find material, falling back to default: "
                  << name << std::endl;
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
}

GLuint MaterialBank::loadTexture(const std::string &path) {
    auto itr = textureTable.find(path);
    if (itr != textureTable.end()) {
        return itr->second;
    }

    int w, h, channels;
    unsigned char *image = stbi_load(path.c_str(), &w, &h, &channels, STBI_default);
    if (!image) {
        std::cerr << "Could not load texture: " << path << std::endl;
        return 0;
    }
    std::cout << "Loaded texture: " << path << std::endl;

    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (channels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    }
    else if (channels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    }
    else {
        std::cerr << "Could not load texture: " << path
                  << ": unsupported channel number: " << channels << std::endl;
        return 0;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    auto error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "Error when loading texture: " << path << ": "
                  << util::error_string(error) << std::endl;
        return 0;
    }

    stbi_image_free(image);
    textureTable[path] = id;
    return id;
}

void MaterialBank::instantiate() {
    if (instance) free();
    instance = new MaterialBank();
}

void MaterialBank::free() {
    delete instance;
}
