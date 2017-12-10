//
// Created by Jean-Luc Thumm on 12/7/17.
//

#include "OBJBank.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/geometric.hpp>

#include "debug.h"

std::vector<OBJBank::Data> OBJBank::table;

const OBJBank::Data &OBJBank::get(refID objId) {
    if (table.empty() || objId > table.size() - 1) {
        throw std::runtime_error{"OBJBank could not find objID"};
    }
    return table[objId];
}

OBJBank::refID OBJBank::load(std::string path) {
    std::ifstream in{path};
    if (!in.is_open()) {
        std::cerr << "Could not open file: " << path
                  << ": " << std::strerror(errno) << std::endl;
        return 0;
    }
    Data data{};

    std::string line, next;
    std::string id;
    float x, y, z;
    std::string f; // face line token

    float minX, minY, minZ;
    float maxX, maxY, maxZ;
    minX = minY = minZ = std::numeric_limits<float>::max();
    maxX = maxY = maxZ = std::numeric_limits<float>::min();

    std::vector<glm::vec3> tempNormals;
    std::vector<std::string> fs;    // face line tokens

    while (getline(in, line)) {
        std::stringstream ss{line};
        ss >> id;

        if (id == "v") {
            ss >> x;
            ss >> y;
            ss >> z;

            if (x < minX) minX = x;
            else if (x > maxX) maxX = x;
            if (y < minY) minY = y;
            else if (y > maxY) maxY = y;
            if (z < minZ) minZ = z;
            else if (z > maxZ) maxZ = z;

            data.vertices.emplace_back(x, y, z);
        }
        else if (id == "vn") {
            ss >> x;
            ss >> y;
            ss >> z;
            glm::vec3 normal{x, y, z};

            tempNormals.push_back(glm::normalize(normal));
        }
        else if (id == "f") {
            // make sure we can align normals with vertices
            if (data.normals.size() < data.vertices.size()) {
                data.normals.resize(data.vertices.size());
            }
            for (int i = 0; i < 3; i++) {
                ss >> f;
                size_t sep = f.find("//");
                std::string indexStr = f.substr(0, sep);
                std::string normalStr = f.substr(sep + 2, f.length());
                auto index = static_cast<unsigned int>(std::stoi(indexStr));
                auto normal = static_cast<unsigned int>(std::stoi(normalStr));

                data.faces.push_back(index - 1);
                data.normals[index - 1] = tempNormals[normal - 1];
            }
        }
    }

    data.max = glm::vec3{maxX, maxY, maxZ};
    data.min = glm::vec3{minX, minY, minZ};

    std::cout << "=> Loaded " << path << " : ";
    std::cout << data.vertices.size() << " vertices ";
    std::cout << data.normals.size() << " normals ";
    std::cout << data.faces.size() << " indices " << std::endl;

    table.push_back(std::move(data));
    return table.size() - 1;
}
