//
// Created by Jean-Luc Thumm on 12/7/17.
//

#include "MeshBank.h"
#include "MaterialBank.hpp"
#include "util.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/geometric.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include "debug.h" // DEBUG

namespace fs = boost::filesystem;
namespace alg = boost::algorithm;

MeshBank *MeshBank::instance;

const std::vector<OBJElement> &MeshBank::get(refID objId) {
    if (table.empty() || objId > table.size() - 1) {
        throw std::runtime_error{"OBJBank could not find objID"};
    }
    return table[objId];
}

MeshBank::refID MeshBank::load(std::string path) {
    std::ifstream in{path};
    if (!in.is_open()) {
        std::cerr << "Could not open file: " << path
                  << ": " << std::strerror(errno) << std::endl;
        return 0;
    }
    fs::path bpath{fs::path{path}.parent_path()};

    std::vector<OBJElement> object;

    std::string line, token;
    float x, y, z;

    float minX, minY, minZ;
    float maxX, maxY, maxZ;
    minX = minY = minZ = std::numeric_limits<float>::max();
    maxX = maxY = maxZ = std::numeric_limits<float>::min();

    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec3> tempNormals;
    std::vector<glm::vec2> tempUvs;
    std::vector<std::string> splitRes;    // face line tokens

    OBJElement element;
    element.material = MaterialBank::I()->get("default");

    while (getline(in, line)) {
        std::stringstream ss{line};
        ss >> token;

        if (token == "mtllib") {
            ss >> token;
            fs::path p = bpath / fs::path{token};
            if (!MaterialBank::I()->parse(p.string())) {
                std::cerr << "Could not parse mtllib:" << p.string() << std::endl;
            }
        }
        else if (token == "o") {
            if (!element.vertices.empty() && put(element)) {
                element.min = glm::vec3{minX, minY, minZ};
                element.max = glm::vec3{maxX, maxY, maxZ};
                element.name = token;
                object.push_back(std::move(element));
            }

            ss >> token;
            element = OBJElement{};
            element.material = MaterialBank::I()->get("default");
            element.name = token;
        }
        else if (token == "v") {
            ss >> x;
            ss >> y;
            ss >> z;

            if (x < minX) minX = x;
            else if (x > maxX) maxX = x;
            if (y < minY) minY = y;
            else if (y > maxY) maxY = y;
            if (z < minZ) minZ = z;
            else if (z > maxZ) maxZ = z;

            tempVertices.emplace_back(x, y, z);
        }
        else if (token == "vn") {
            ss >> x;
            ss >> y;
            ss >> z;

            tempNormals.emplace_back(x, y, z);
        }
        else if (token == "vt") {
            ss >> x;
            ss >> y;

            tempUvs.emplace_back(x, y);
        }
        else if (token == "f") {
            for (int i = 0; i < 3; i++) {
                // split by /
                ss >> token;
                splitRes.clear();
                alg::split(splitRes, token, alg::is_any_of("/"));
                if (splitRes.size() != 3) {
                    std::cerr << "invalid face: " << line << std::endl;
                    continue;
                } // f xx/xx/xx

                int vindex = std::stoi(splitRes[0]) - 1;
                element.vertices.push_back(tempVertices[vindex]);
                if (!splitRes[1].empty()) {
                    int uvIndex = std::stoi(splitRes[1]) - 1;
                    element.uvs.push_back(tempUvs[uvIndex]);
                }
                if (!splitRes[2].empty()) {
                    int normIndex = std::stoi(splitRes[2]) - 1;
                    element.normals.push_back(tempNormals[normIndex]);
                }
            }

            // check for ngon
            if (!ss.eof()) {
                std::cerr << "Found ngon in file: " << path;
                std::cerr << ": " << line << std::endl;
            }
        }
        else if (token == "usemtl") {
            ss >> token;
            element.material = MaterialBank::I()->get(token);
        }
    }

    // last element still needs to be added
    if (put(element)) {
        element.min = glm::vec3{minX, minY, minZ};
        element.max = glm::vec3{maxX, maxY, maxZ};
        element.name = token;
        object.push_back(std::move(element));
    }

    unsigned long vertc, normc, uvc, facec;
    vertc = normc = uvc = facec = 0;
    for (auto &sub : object) {
        vertc += sub.vertices.size();
        normc += sub.normals.size();
        uvc += sub.uvs.size();
        facec += sub.indices.size();
    }

    using std::endl;
    using std::cout;

    cout << "Loaded object: " << path << endl;
    cout << "\telements:\t" << object.size() << endl;
    cout << "\tvertices:\t" << vertc << endl;
    cout << "\tnormals:\t" << normc << endl;
    cout << "\tindices:\t" << facec << endl;

    table.push_back(std::move(object));

    return table.size() - 1;
}

MeshBank *MeshBank::I() {
    return instance;
}

bool MeshBank::put(OBJElement &element) {
    GLuint EBO, vertexVBO, normalVBO, uvVBO;

    glGenVertexArrays(1, &element.VAO);
//    glGenBuffers(1, &EBO);
    glGenBuffers(1, &vertexVBO);
    glGenBuffers(1, &normalVBO);
    glGenBuffers(1, &uvVBO);

    auto vsize = static_cast<GLsizeiptr>(element.vertices.size() * sizeof(glm::vec3));
    auto nsize = static_cast<GLsizeiptr>(element.normals.size() * sizeof(glm::vec3));
    auto usize = static_cast<GLsizeiptr>(element.uvs.size() * sizeof(glm::vec2));
//    auto esize = static_cast<GLsizeiptr>(element.indices.size() * sizeof(unsigned int));

    glBindVertexArray(element.VAO);

    // positions
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, vsize, &element.vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

    // normals
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, nsize, &element.normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(element.vertices.size()));

    // uvs
    glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
    glBufferData(GL_ARRAY_BUFFER, usize, &element.uvs[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

    // indices
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, esize, &element.indices[0], GL_STATIC_DRAW);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // DEBUG
    util::print("vertices", element.vertices);
    util::print("normals", element.normals);
    util::print("uvs", element.uvs);

    auto error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "Error when loading obj element: "
                  << util::error_string(error) << std::endl;
        return false;
    }
    return true;
}

void MeshBank::instantiate() {
    if (instance) free();
    instance = new MeshBank;
}

void MeshBank::free() {
    delete instance;
}

